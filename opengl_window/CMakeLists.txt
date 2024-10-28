cmake_minimum_required(VERSION 3.10)
project(opengl_test)

add_executable(
    opengl_test
    main.cpp
    deps/glad/src/glad.c
)

add_subdirectory(deps/glfw)

include_directories(deps/glad)
include_directories(deps/glad/include)
include_directories(deps/glfw/include)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF "" FORCE)

target_link_libraries(opengl_test glfw)
target_link_libraries(opengl_test OpenGL::GL)