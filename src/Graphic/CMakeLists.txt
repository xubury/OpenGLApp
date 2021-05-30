set(Include_Root ${PROJECT_SOURCE_DIR}/include/Graphic)
set(Src_Root ${PROJECT_SOURCE_DIR}/src/Graphic)

if (POLICY CMP0072)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif()
find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
set_target_properties(assimp::assimp PROPERTIES MAP_IMPORTED_CONFIG_DEBUG Release)

set(Graphic_Include
    ${Include_Root}/Export.hpp
    ${Include_Root}/OpenGL.hpp
    ${Include_Root}/Drawable.hpp
    ${Include_Root}/CameraBase.hpp
    ${Include_Root}/Shader.hpp
    ${Include_Root}/Texture.hpp
    ${Include_Root}/Vertex.hpp
    ${Include_Root}/TextureArray.hpp
    ${Include_Root}/VertexBuffer.hpp
    ${Include_Root}/ElementBuffer.hpp
    ${Include_Root}/RenderTarget.hpp
    ${Include_Root}/RenderStates.hpp
    ${Include_Root}/Primitive.hpp
    ${Include_Root}/Mesh.hpp
    ${Include_Root}/Model.hpp
    ${Include_Root}/FrameBuffer.hpp)

set(Graphic_Src
    ${Src_Root}/CameraBase.cpp
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
    ${Src_Root}/FrameBuffer.cpp)

add_library(glgraphic ${Graphic_Src})

set_property(TARGET glgraphic PROPERTY CXX_STANDARD 17)

target_compile_options(glgraphic PRIVATE -Wall -Wextra -Wundef -pedantic)

target_compile_definitions(glgraphic PRIVATE GRAPHIC_EXPORTS)

target_link_libraries(glgraphic PUBLIC
    OpenGL::GL
    glbinding::glbinding
    glm
    assimp::assimp)
if(UNIX)
    target_link_libraries(glgraphic PUBLIC X11 Xrandr Xi dl pthread)
endif()

target_include_directories(glgraphic PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs)

target_include_directories(glgraphic PUBLIC ${GLM_INCLUDE_DIRS})

install(TARGETS glgraphic LIBRARY
    DESTINATION lib)

install(FILES ${Graphic_Include} DESTINATION include/Graphic)
