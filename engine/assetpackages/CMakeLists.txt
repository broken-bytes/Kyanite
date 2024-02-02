FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.cxx")

add_library(AssetPackages SHARED ${SRC} ${HEADERS})

find_package(cereal CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_include_directories(AssetPackages PRIVATE include)
target_include_directories(AssetPackages PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)
target_include_directories(AssetPackages PRIVATE ${CMAKE_SOURCE_DIR}/engine/audio/include)
target_include_directories(AssetPackages PRIVATE ${CMAKE_SOURCE_DIR}/engine/rendering/include)
target_include_directories(AssetPackages PRIVATE ${CMAKE_SOURCE_DIR}/engine/input/include)
target_include_directories(AssetPackages PRIVATE ${CMAKE_SOURCE_DIR}/engine/math/include)
target_include_directories(AssetPackages PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)

target_link_libraries(AssetPackages PRIVATE Core Audio Rendering Input cereal::cereal nlohmann_json::nlohmann_json)
