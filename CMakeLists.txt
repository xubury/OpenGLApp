cmake_minimum_required(VERSION 3.0.0)

project(opengl VERSION 0.1.0)

if (POLICY CMP0072)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)


add_executable(OpenGLApp libs/glad.c src/main.cpp src/Application.cpp
    src/RenderWindow.cpp 
    src/Shader.cpp 
    src/Texture.cpp 
    src/Drawable.cpp 
    src/Cube.cpp)

set_property(TARGET OpenGLApp PROPERTY CXX_STANDARD 17)

target_compile_options(OpenGLApp PUBLIC -Wall -Wextra -Wundef -pedantic)
target_link_libraries(OpenGLApp OpenGL::GL X11 pthread Xrandr Xi dl glm glfw)

target_include_directories(OpenGLApp PUBLIC include libs)
