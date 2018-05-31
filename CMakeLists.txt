cmake_minimum_required (VERSION 2.6)

project (HelloOpenGL)

option(HZ_BUILD_COMMON_LIBS "Build common libs?" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)  # for static libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)  # for dynamic libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "BUILD START...")
message(STATUS "PATH:")
message(STATUS "  SYSTEM = ${CMAKE_SYSTEM_NAME}")
message(STATUS "  ROOT = CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "  CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "  CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "  CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "OPTIONS:")
message(STATUS "  HZ_BUILD_COMMON_LIBS = ${HZ_BUILD_COMMON_LIBS}")
message(STATUS "")

##################################################################
# HZ_BUILD_COMMON_LIBS
##################################################################

if(HZ_BUILD_COMMON_LIBS)

	# LIB: GLEW
	set(BUILD_UTILS OFF CACHE BOOL "utilities")
	add_subdirectory(lib/glew/build/cmake)

	# LIB: GLFW
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
	set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
	set(GLFW_DOCUMENT_INTERNALS OFF CACHE BOOL "Include internals in documentation")
	add_subdirectory(lib/glfw)

	# LIB: GLM
	set(GLM_STATIC_LIBRARY_ENABLE ON CACHE BOOL "GLM static library")
	add_subdirectory(lib/glm)
endif()
unset(HZ_BUILD_COMMON_LIBS CACHE)

##################################################################
# SET LIBS
##################################################################

find_package(OpenGL REQUIRED)

if(WIN32)
	set(COMMON_LIBS ${OPENGL_LIBRARIES} glew glfw)
	message(STATUS "WIN32 add libs: " ${COMMON_LIBS} )
elseif (UNIX)
	if(APPLE)
		set(COMMON_LIBS ${OPENGL_LIBRARIES} glew glfw glm stdc++ "-framework Foundation" "-framework Cocoa" "-framework OpenGL" "-framework CoreVideo" "-framework IOKit")
		message(STATUS "APPLE add libs: " ${COMMON_LIBS} )
	else()
		set(COMMON_LIBS vermilion ${OPENGL_LIBRARIES} glew glfw GL rt dl)
		message(STATUS "UNIX add libs: " ${COMMON_LIBS} )
	endif()
else()
	set(COMMON_LIBS)
	message(STATUS "OTHER SYSTEM add libs: " ${COMMON_LIBS})
endif()

##################################################################
# 
##################################################################

include_directories(${CMAKE_SOURCE_DIR}/lib/glew/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/glm)

link_directories(${CMAKE_SOURCE_DIR}/build/lib)

set(EXAMPLES
	00-hello
	01-shaders
	02-more_vertices
	03-rotate
)

foreach(EXAMPLE ${EXAMPLES})
	add_executable(${EXAMPLE} WIN32 src/${EXAMPLE}/${EXAMPLE}.cpp ${ASSET_FILES} ${COMMON_HEADERS})
	set_property(TARGET ${EXAMPLE} PROPERTY DEBUG_POSTFIX _d)
	target_link_libraries(${EXAMPLE} ${COMMON_LIBS})
endforeach(EXAMPLE)

ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})

message(STATUS "BUILD DONE")
