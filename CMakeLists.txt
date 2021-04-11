cmake_minimum_required(VERSION 3.0.0)

project(opengl VERSION 0.1.0)

if (POLICY CMP0072)
    set(OpenGL_GL_PREFERENCE GLVND)
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)


add_executable(OpenGLApp src/main.cpp src/glad.c src/GLWindow.cpp src/Shader.cpp)

set_property(TARGET OpenGLApp PROPERTY CMAKE_CXX_STANDARD 17)

target_compile_options(OpenGLApp PUBLIC -Wall -Wextra -Wundef -pedantic)
target_link_libraries(OpenGLApp OpenGL::GL X11 pthread Xrandr Xi dl glm glfw)
target_include_directories(OpenGLApp PUBLIC include)
