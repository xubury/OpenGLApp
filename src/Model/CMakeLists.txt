set(Include_Root ${PROJECT_SOURCE_DIR}/include/Model)
set(Src_Root ${PROJECT_SOURCE_DIR}/src/Model)

find_package(glm REQUIRED)
find_package(assimp REQUIRED)

set(Model_Include
    ${Include_Root}/Model.hpp
    ${Include_Root}/Bone.hpp
    ${Include_Root}/Animation.hpp
    ${Include_Root}/Animator.hpp
    ${Include_Root}/Mesh.hpp)

set(Model_Src
    ${Src_Root}/Model.cpp
    ${Src_Root}/Bone.cpp
    ${Src_Root}/Animation.cpp
    ${Src_Root}/Animator.cpp
    ${Src_Root}/Mesh.cpp)

add_library(te-model ${Model_Src})

set_property(TARGET te-model PROPERTY CXX_STANDARD 17)

target_compile_options(te-model PRIVATE -Wall -Wextra -Wundef -pedantic)

target_compile_definitions(te-model PRIVATE TE_EXPORT)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(te-model PRIVATE TE_SHARED)
endif()

target_link_libraries(te-model
    PUBLIC
    te-core
    te-graphic
    glm
    assimp::assimp)

target_include_directories(te-model PUBLIC ${PROJECT_SOURCE_DIR}/include)

install(TARGETS te-model LIBRARY DESTINATION lib)

install(FILES ${Model_Include} DESTINATION include/Model)
