cmake_minimum_required(VERSION 3.0.0)

project(opengl VERSION 0.1.0)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
endif()

add_subdirectory(src/Core)
add_subdirectory(src/Graphic)
add_subdirectory(src/ECS)

set(Editor_Src
    libs/imgui_impl_glfw.cpp
    libs/imgui_impl_opengl3.cpp
    src/Editor/EditorLayer.cpp
    src/Editor/EditorContext.cpp
    src/Editor/EditorCamera.cpp)

set(App_Src
    src/Apps/Application.cpp
    src/Apps/SandboxApp.cpp)

set(Layers_Src
    src/Layers/SandboxLayer.cpp
    src/Layers/ShadowLayer.cpp
    src/Layers/GBufferLayer.cpp
    src/Layers/SkyboxLayer.cpp)

set(Component_Src
    src/Component/BoundingBox.cpp
    src/Component/ShadowMap.cpp)

set(Entity_Src
    src/Entity/EntityBase.cpp
    src/Entity/ModelEntity.cpp
    src/Entity/Cube.cpp
    src/Entity/Light.cpp
    src/Entity/Sphere.cpp
    src/Entity/Terrain.cpp)

set(Window_Src
    src/Window/RenderWindow.cpp
    src/Window/Keyboard.cpp
    src/Window/Mouse.cpp
    src/Window/Action.cpp)

set(Physics_Src
    src/Physics/PhysicsWorld.cpp
    src/Physics/Collision.cpp
    src/Physics/ImpulseSolver.cpp
    src/Physics/Rigidbody.cpp
    src/Physics/TerrainCollider.cpp
    src/Physics/SphereCollider.cpp
    src/Physics/HullCollider.cpp
    src/Physics/GJK.cpp
    src/Physics/PositionSolver.cpp)

if (NOT BUILD_LIBS_ONLY)

find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(OpenGLApp
    src/main.cpp
    ${App_Src}
    ${Layers_Src}
    ${Component_Src}
    ${Entity_Src}
    ${Window_Src}
    ${Physics_Src}
    ${Editor_Src})

set_property(TARGET OpenGLApp PROPERTY CXX_STANDARD 17)
target_compile_options(OpenGLApp PUBLIC -Wall -Wextra -Wundef -pedantic)
target_link_libraries(OpenGLApp
    te-graphic
    glfw
    imgui::imgui)
target_include_directories(OpenGLApp PUBLIC include libs)
endif()

