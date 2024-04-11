cmake_minimum_required(VERSION 3.0.0)
project(opengl_project VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(vendor)

# Renderer is a function class, which means you cannot instantiate the class.
# Use init() to initialize it and start to use, and shutdown() when terminate it.
# when rendering objects, some vertices will be rendered 3 or more times. I don't
# think this is a good idea, but for now, just make it work.
add_library(
    renderer
    src/Renderer.cpp
    src/Renderer.h
)

# opengl function bug detect tool
# refer to Cherno's OpenGL tutorial
add_library(
    errorcheck
    src/ErrorCheck.h
    src/ErrorCheck.cpp
)

# .obj file loader
# store object data in cpu heap
# only deal with v, vt, vn and f
add_library(
    objloader
    src/ObjLoader.h
    src/ObjLoader.cpp
)

# Camera library for create and control cameras
# can only control direction in version 1 (i will update soon)
add_library(
    camera
    src/Camera.h
    src/Camera.cpp
)

add_executable(opengl_project src/window.cpp)

target_link_libraries(
    opengl_project
    vendor
    glfw
    renderer
    errorcheck
    objloader
    camera
)

include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/vendor)