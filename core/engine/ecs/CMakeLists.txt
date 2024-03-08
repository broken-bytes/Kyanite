FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.cxx")

add_library(EntityComponentSystem SHARED ${SRC} ${HEADERS})

target_include_directories(EntityComponentSystem PRIVATE include)
target_include_directories(EntityComponentSystem PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_include_directories(EntityComponentSystem PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/io/include)

find_package(flecs CONFIG REQUIRED)
target_link_libraries(EntityComponentSystem PRIVATE flecs::flecs_static)
target_link_libraries(EntityComponentSystem PRIVATE IO)