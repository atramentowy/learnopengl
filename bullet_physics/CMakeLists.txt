cmake_minimum_required(VERSION 3.10)

# Project name and version
project(opengl_test VERSION 1.0.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Bullet Physics

set(BULLET_PHYSICS_DIR "${CMAKE_SOURCE_DIR}/deps/bullet3")

# Bullet source directories
set(BULLET_INCLUDE_DIRS ${BULLET_PHYSICS_DIR}/src)

set(BULLET_LIBRARIES
    ${BULLET_PHYSICS_DIR}/build/lib/
)

include_directories(${BULLET_INCLUDE_DIRS})

# Add the executable for your project (main.cpp and all other source files)
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${GLAD_SOURCES}
)

# Link libraries
# Link glfw and Bullet libraries consistently
target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE ${BULLET_LIBRARIES}
)

# Print helpful messages
message(STATUS "Bullet include directories: ${BULLET_INCLUDE_DIRS}")
message(STATUS "Bullet libraries: ${BULLET_LIBRARIES}")

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
