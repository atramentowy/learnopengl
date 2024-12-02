cmake_minimum_required(VERSION 3.10)

# Project name and version
project(opengl_test VERSION 1.0.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW setup
# Add GLFW as a subdirectory (assuming you have the GLFW source code in `external/glfw`)
add_subdirectory(deps/glfw)

# ImGui setup
# Set the path to the ImGui directory
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/deps/imgui)

# Collect all ImGui source files
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include ImGui headers
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

# GLAD setup
# Add GLAD source files (Assuming you have GLAD in `external/glad`)
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/deps/glad)
file(GLOB GLAD_SOURCES
    ${GLAD_DIR}/src/glad.c
)

# Include GLAD headers
include_directories(${GLAD_DIR}/include)

# Add the executable for your project (main.cpp and all other source files)
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${IMGUI_SOURCES}
    ${GLAD_SOURCES}
)

# Link GLFW to the project
target_link_libraries(${PROJECT_NAME} glfw)

# Link OpenGL (on Linux/Unix, you may need to manually link OpenGL)
if (APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(${PROJECT_NAME} ${OpenGL_LIBRARY})
elseif (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()