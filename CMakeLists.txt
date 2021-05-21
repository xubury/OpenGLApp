cmake_minimum_required(VERSION 3.0.0)

project(opengl VERSION 0.1.0)

if (POLICY CMP0072)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(imgui REQUIRED)
set_target_properties(assimp::assimp PROPERTIES MAP_IMPORTED_CONFIG_DEBUG Release)

set(Editor_Src
    src/Editor/Editor.cpp)

set(Component_Src
    src/Component/Transfrom.cpp
    src/Component/BoundingBox.cpp)

set(Entity_Src
    src/Entity/Camera.cpp
    src/Entity/Model.cpp
    src/Entity/Cube.cpp)

set(Utility_Src
    src/Utility/Memory.cpp
    src/Utility/Time.cpp)

set(ECS_Src
    src/ECS/Entity.cpp)

set(Graphic_Src
    src/Graphic/Shader.cpp
    src/Graphic/Texture.cpp
    src/Graphic/Vertex.cpp
    src/Graphic/TextureArray.cpp
    src/Graphic/VertexBuffer.cpp
    src/Graphic/ElementBuffer.cpp
    src/Graphic/RenderWindow.cpp
    src/Graphic/RenderTarget.cpp
    src/Graphic/RenderStates.cpp
    src/Graphic/Axis.cpp
    src/Graphic/Mesh.cpp
    src/Graphic/ModelResource.cpp
    src/Graphic/FrameBuffer.cpp)

set(Window_Src
    src/Window/GlContext.cpp
    src/Window/Keyboard.cpp
    src/Window/Mouse.cpp
    src/Window/Action.cpp)

set(Library_Src
    libs/glad.c
    libs/imgui_impl_glfw.cpp
    libs/imgui_impl_opengl3.cpp)

add_executable(OpenGLApp
    src/main.cpp
    src/Game.cpp
    ${Library_Src}
    ${Utility_Src}
    ${ECS_Src}
    ${Component_Src}
    ${Entity_Src}
    ${Graphic_Src}
    ${Window_Src}
    ${Editor_Src})

set_property(TARGET OpenGLApp PROPERTY CXX_STANDARD 17)

target_compile_options(OpenGLApp PUBLIC -Wall -Wextra -Wundef -pedantic)

target_link_libraries(OpenGLApp
    OpenGL::GL
    glm glfw
    assimp::assimp
    imgui::imgui)
if(UNIX)
    target_link_libraries(OpenGLApp x11 xrandr Xi dl pthread)
endif()

target_include_directories(OpenGLApp PUBLIC include libs)
