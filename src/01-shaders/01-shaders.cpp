//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

// https://habr.com/post/311808/

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

/****************************************************************************************/

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/****************************************************************************************/

GLuint CreateShader(const GLchar* shaderSource, const GLenum shaderType)
{
    GLint success;
    GLchar infoLog[512];
    
    GLuint shader = glCreateShader(shaderType); // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR:: CreateShader:: \n" << infoLog << std::endl;
        return 0;
    }
    
    return shader;
}

GLuint LinkProgram(GLuint shaderV, GLuint shaderF)
{
    GLint success;
    GLchar infoLog[512];
    
    GLuint program = glCreateProgram();
    glAttachShader(program, shaderV);
    glAttachShader(program, shaderF);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR:: LinkProgram:: \n" << infoLog << std::endl;
        return 0;
    }
    
    return program;
}

/****************************************************************************************/

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = ""
    " #version 330 core"
    " "
    " layout (location = 0) in vec3 position;"
    " "
    " out vec4 vertexColor; // Передаем цвет во фрагментный шейдер \n"
    " "
    " void main()"
    " {"
    "    gl_Position = vec4(position.x, position.y, position.z, 1.0);"
    "    vertexColor = vec4(0.5f, 1.0f, 0.0f, 1.0f);"
    " }";

const GLchar* fragmentShaderSource = ""
    " #version 330 core"
    " "
    " in vec4 vertexColor; // Входная переменная из вершинного шейдера (то же название и тот же тип) \n"
    " "
    " out vec4 color;"
    " "
    " void main()"
    " {"
    "    color = vertexColor;"
    " }";

/****************************************************************************************/

int main()
{
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    //Если вы используете Mac OS X, то необходимо добавить
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Init GLEW
    glewExperimental = GL_TRUE;   // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewInit();                   // Initialize GLEW to setup the OpenGL Function pointers

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    /***********/
    
    // Create shaders
    GLuint vertexShader = CreateShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = CreateShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	// Link shader program
    GLuint shaderProgram = LinkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /***********/

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left
         0.5f, -0.5f, 0.0f, // Right
         0.0f,  0.5f, 0.0f  // Top
    };
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	glUseProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}















