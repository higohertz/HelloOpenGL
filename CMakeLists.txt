cmake_minimum_required (VERSION 2.6)

project (HelloOpenGL)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)  # for static libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)  # for dynamic libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "BUILD START...")
message(STATUS "  SYSTEM = ${CMAKE_SYSTEM_NAME}")
message(STATUS "  ROOT = CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "  CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "  CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "  CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

##################################################################
# GLEW
##################################################################

#set(BUILD_UTILS OFF CACHE BOOL "utilities")

include_directories(lib/glew/include)
add_subdirectory(lib/glew/build/cmake)

##################################################################
# GLFW
##################################################################

#set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
#set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
#set(GLFW_DOCUMENT_INTERNALS OFF CACHE BOOL "Include internals in documentation")

include_directories(lib/glfw/include)
add_subdirectory(lib/glfw)

##################################################################
# 
##################################################################

link_directories(${CMAKE_BINARY_DIR}/build/lib)

##################################################################
# 
##################################################################

message(STATUS "BUILD DONE")
