FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.c*")

find_package(SDL2 REQUIRED)
find_package(unofficial-sqlite3 REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(EditorApp ${SRC} ${HEADERS})

target_include_directories(EditorApp PRIVATE include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/assetpackages/include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/editor//core/include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/audio/include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/ecs/include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/logger/include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/core/engine//core/include)
target_include_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/rendering/include)

target_link_directories(EditorApp PRIVATE ${CMAKE_SOURCE_DIR}/.build/debug)

target_link_libraries(EditorApp PRIVATE SDL2::SDL2)

add_custom_command(TARGET EditorApp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:EditorApp> $<TARGET_RUNTIME_DLLS:EditorApp>
  COMMAND_EXPAND_LISTS
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${HEADERS})


add_custom_command(
    TARGET EditorApp
    POST_BUILD
    COMMAND swift build -Xlinker ${CMAKE_BINARY_DIR}/engine/assetpackages/debug/AssetPackages.lib
    -Xlinker ${CMAKE_BINARY_DIR}/engine/audio/debug/Audio.lib
    -Xlinker ${CMAKE_BINARY_DIR}/engine/core/debug/Core.lib
    -Xlinker ${CMAKE_BINARY_DIR}/engine/ecs/debug/EntityComponentSystem.lib
    -Xlinker ${CMAKE_BINARY_DIR}/engine/input/debug/Input.lib
    -Xlinker ${CMAKE_BINARY_DIR}/engine/rendering/debug/Rendering.lib
    -Xlinker ${CMAKE_BINARY_DIR}/editor/core/debug/EditorCore.lib
    -Xlinker ${CMAKE_BINARY_DIR}/editor/assetpipeline/debug/AssetPipeline.lib
    --static-swift-stdlib
COMMENT "Running swift build with custom flags..."
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/.build/debug/KyaniteEditor.dll
            $<TARGET_RUNTIME_DLLS:EditorApp>/KyaniteEditor.dll
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/.build/debug/KyaniteEngine.dll
            $<TARGET_RUNTIME_DLLS:EditorApp>/KyaniteEngine.dll
)