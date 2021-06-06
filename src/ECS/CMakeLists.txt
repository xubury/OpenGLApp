set(Include_Root ${PROJECT_SOURCE_DIR}/include/ECS)
set(Src_Root ${PROJECT_SOURCE_DIR}/src/ECS)

set(ECS_Include
    ${Include_Root}/Application.hpp
    ${Include_Root}/Component.hpp
    ${Include_Root}/Defines.hpp
    ${Include_Root}/ECS.hpp
    ${Include_Root}/Entity.hpp
    ${Include_Root}/EntityManager.hpp
    ${Include_Root}/System.hpp)

add_library(te-ecs INTERFACE)

install(FILES ${ECS_Include} DESTINATION include/ECS)

set_property(TARGET te-ecs PROPERTY CXX_STANDARD 17)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(te-ecs PUBLIC TE_SHARED)
endif()
