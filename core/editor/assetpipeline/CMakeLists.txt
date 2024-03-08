FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.c*")

find_package(unofficial-sqlite3 REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(AssetPipeline SHARED ${SRC} ${HEADERS})

target_include_directories(AssetPipeline PRIVATE include)
target_include_directories(AssetPipeline PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/audio/include)
target_include_directories(AssetPipeline PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/ecs/include)
target_include_directories(AssetPipeline PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_include_directories(AssetPipeline PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/logger/include)
target_include_directories(AssetPipeline PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/io/include)
target_include_directories(AssetPipeline PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/rendering/include)

target_link_libraries(AssetPipeline PRIVATE unofficial::sqlite3::sqlite3 assimp::assimp)

# Link all engine libraries
target_link_libraries(AssetPipeline PRIVATE Logger EntityComponentSystem Audio IO Rendering)

add_custom_command(TARGET AssetPipeline POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:AssetPipeline> $<TARGET_RUNTIME_DLLS:AssetPipeline>
  COMMAND_EXPAND_LISTS
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${HEADERS})
