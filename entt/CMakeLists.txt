cmake_minimum_required(VERSION 3.10)

# Project name and version
project(opengl_test VERSION 1.0.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Debug")

# GLFW setup
# Add GLFW as a subdirectory (assuming you have the GLFW source code in `external/glfw`)
add_subdirectory(deps/glfw)

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
    ${GLAD_SOURCES}
)

# Entt
add_subdirectory(deps/entt)

# Link glfw and Bullet libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE EnTT
)

# optional but not required for header-only libraries
# target_include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/deps/entt/src)

# Link OpenGL (on Linux/Unix, you may need to manually link OpenGL)
if (APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(${PROJECT_NAME} ${OpenGL_LIBRARY})
elif (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
elif (WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()
