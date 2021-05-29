cmake_minimum_required(VERSION 3.0.0)

project(opengl VERSION 0.1.0)

find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(src/Graphic)

set(Editor_Src
    libs/imgui_impl_glfw.cpp
    libs/imgui_impl_opengl3.cpp
    src/Editor/Editor.cpp
    src/Editor/RenderContext.cpp)

set(Component_Src
    src/Component/Transfrom.cpp
    src/Component/BoundingBox.cpp)

set(Entity_Src
    src/Entity/EntityBase.cpp
    src/Entity/Camera.cpp
    src/Entity/ModelEntity.cpp
    src/Entity/Cube.cpp)

set(Utility_Src
    src/Utility/Memory.cpp
    src/Utility/Time.cpp)


set(Window_Src
    src/Window/RenderWindow.cpp
    src/Window/GLContext.cpp
    src/Window/Keyboard.cpp
    src/Window/Mouse.cpp
    src/Window/Action.cpp)

if (NOT BUILD_LIBS_ONLY)
add_executable(OpenGLApp
    src/main.cpp
    src/Game.cpp
    ${Utility_Src}
    ${Component_Src}
    ${Entity_Src}
    ${Window_Src}
    ${Editor_Src})

set_property(TARGET OpenGLApp PROPERTY CXX_STANDARD 17)
target_compile_options(OpenGLApp PUBLIC -Wall -Wextra -Wundef -pedantic)
target_link_libraries(OpenGLApp glgraphic glfw imgui::imgui)
target_include_directories(OpenGLApp PUBLIC include libs)
endif()
