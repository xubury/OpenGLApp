find_package(spdlog REQUIRED)

set(Include_Root ${PROJECT_SOURCE_DIR}/include/Core)
set(Src_Root ${PROJECT_SOURCE_DIR}/src/Core)


set(Core_Include
    ${Include_Root}/PlatformDetection.hpp
    ${Include_Root}/Config.hpp
    ${Include_Root}/Export.hpp
    ${Include_Root}/Log.hpp
    ${Include_Root}/Assert.hpp
    ${Include_Root}/Base.hpp
    ${Include_Root}/ResourceManager.hpp)

set(Core_Src
    ${Src_Root}/Log.cpp)

add_library(te-core
    ${Core_Src})

set_property(TARGET te-core PROPERTY CXX_STANDARD 17)

target_compile_options(te-core PRIVATE -Wall -Wextra -Wundef -pedantic)

target_compile_definitions(te-core PRIVATE TE_EXPORT)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(te-core PUBLIC TE_SHARED)
endif()

target_link_libraries(te-core PUBLIC spdlog::spdlog)

target_include_directories(te-core PUBLIC
    ${PROJECT_SOURCE_DIR}/include)

target_include_directories(te-core PRIVATE
    ${PROJECT_SOURCE_DIR}/libs)

install(TARGETS te-core LIBRARY DESTINATION lib)

install(FILES ${Core_Include} DESTINATION include/Core)
