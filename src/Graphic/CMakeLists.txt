set(Src_Root ${PROJECT_SOURCE_DIR}/src/Graphic)

if (POLICY CMP0072)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(imgui REQUIRED)
set_target_properties(assimp::assimp PROPERTIES MAP_IMPORTED_CONFIG_DEBUG Release)

set(Graphic_Src
    ${Src_Root}/Shader.cpp
    ${Src_Root}/Texture.cpp
    ${Src_Root}/Vertex.cpp
    ${Src_Root}/TextureArray.cpp
    ${Src_Root}/VertexBuffer.cpp
    ${Src_Root}/ElementBuffer.cpp
    ${Src_Root}/RenderTarget.cpp
    ${Src_Root}/RenderStates.cpp
    ${Src_Root}/Primitive.cpp
    ${Src_Root}/Mesh.cpp
    ${Src_Root}/Model.cpp
    ${Src_Root}/FrameBuffer.cpp
    ${PROJECT_SOURCE_DIR}/libs/glad.c)

add_library(gl-graphic ${Graphic_Src})

set_property(TARGET gl-graphic PROPERTY CXX_STANDARD 17)

target_compile_options(gl-graphic PUBLIC -Wall -Wextra -Wundef -pedantic)

target_link_libraries(gl-graphic
    OpenGL::GL
    glm
    glfw
    assimp::assimp
    imgui::imgui)
if(UNIX)
    target_link_libraries(gl-graphic X11 Xrandr Xi dl pthread)
endif()

target_include_directories(gl-graphic PUBLIC 
    ${PROJECT_SOURCE_DIR}/include 
    ${PROJECT_SOURCE_DIR}/libs)
