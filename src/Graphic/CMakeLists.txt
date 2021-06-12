set(Include_Root ${PROJECT_SOURCE_DIR}/include/Graphic)
set(Src_Root ${PROJECT_SOURCE_DIR}/src/Graphic)

find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

set(Graphic_Include
    ${Include_Root}/OpenGL.hpp
    ${Include_Root}/GLContext.hpp
    ${Include_Root}/Buffer.hpp
    ${Include_Root}/Layout.hpp
    ${Include_Root}/Renderer.hpp
    ${Include_Root}/VertexArray.hpp
    ${Include_Root}/Drawable.hpp
    ${Include_Root}/Camera.hpp
    ${Include_Root}/Shader.hpp
    ${Include_Root}/Texture.hpp
    ${Include_Root}/Vertex.hpp
    ${Include_Root}/ModelTextures.hpp
    ${Include_Root}/Primitive.hpp
    ${Include_Root}/Mesh.hpp
    ${Include_Root}/Model.hpp
    ${Include_Root}/FrameBuffer.hpp
    ${Include_Root}/LightBase.hpp)

set(Graphic_Src
    ${Src_Root}/GLContext.cpp
    ${Src_Root}/Buffer.cpp
    ${Src_Root}/Layout.cpp
    ${Src_Root}/Renderer.cpp
    ${Src_Root}/VertexArray.cpp
    ${Src_Root}/Camera.cpp
    ${Src_Root}/Shader.cpp
    ${Src_Root}/Texture.cpp
    ${Src_Root}/Vertex.cpp
    ${Src_Root}/Primitive.cpp
    ${Src_Root}/Mesh.cpp
    ${Src_Root}/Model.cpp
    ${Src_Root}/ModelTextures.cpp
    ${Src_Root}/FrameBuffer.cpp)

add_library(te-graphic
    ${Graphic_Src}
    ${PROJECT_SOURCE_DIR}/libs/stb_image.cpp)

set_property(TARGET te-graphic PROPERTY CXX_STANDARD 17)

target_compile_options(te-graphic PRIVATE -Wall -Wextra -Wundef -pedantic)

target_compile_definitions(te-graphic PRIVATE TE_EXPORT)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(te-core PUBLIC TE_SHARED)
endif()


target_link_libraries(te-graphic 
    PUBLIC
    te-core
    OpenGL::GL
    glbinding::glbinding
    glm
    PRIVATE
    assimp::assimp)
if(UNIX)
    target_link_libraries(te-graphic PUBLIC X11 Xrandr Xi dl pthread)
endif()

target_include_directories(te-graphic PRIVATE
    ${PROJECT_SOURCE_DIR}/libs)

target_include_directories(te-graphic PUBLIC
    ${GLM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include)


install(TARGETS te-graphic LIBRARY DESTINATION lib)

install(FILES ${Graphic_Include} DESTINATION include/Graphic)
