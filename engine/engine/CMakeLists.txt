FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.cxx")

add_library(Engine SHARED ${SRC} ${HEADERS})

target_include_directories(Engine PRIVATE include)
target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)
target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)
target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/engine/audio/include)
target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/engine/rendering/include)
target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/engine/input/include)
target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/engine/math/include)
target_include_directories(Engine PRIVATE ${CMAKE_SOURCE_DIR}/engine/AssetPackages/include)

find_package(SDL2 CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
target_link_libraries(Engine PRIVATE SDL2::SDL2 flecs::flecs_static)
target_link_libraries(Engine PRIVATE AssetPackages Core Audio Rendering Input Math)