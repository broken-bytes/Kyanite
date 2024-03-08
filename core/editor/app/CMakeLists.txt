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

target_link_libraries(EditorApp PRIVATE SDL2::SDL2 SDL2::SDL2main)

add_custom_command(TARGET EditorApp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:EditorApp> $<TARGET_RUNTIME_DLLS:EditorApp>
  COMMAND_EXPAND_LISTS
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${HEADERS})


add_custom_command(
    TARGET EditorApp
POST_BUILD
	 COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/core/engine/audio/Debug/Audio.dll
            ${CMAKE_BINARY_DIR}/core/editor/app/Debug/Audio.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/core/engine/ecs/Debug/EntityComponentSystem.dll
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/EntityComponentSystem.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/core/engine/input/Debug/Input.dll
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/Input.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/core/engine/rendering/Debug/Rendering.dll
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/Rendering.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/core/editor/assetpipeline/Debug/AssetPipeline.dll
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/AssetPipeline.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/core/engine/io/Debug/IO.dll
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/IO.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/core/engine/imgui/Debug/ImGui.dll
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/ImGui.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/core/engine/logger/Debug/Logger.dll
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/Logger.dll
)

add_custom_command(
    TARGET EditorApp 
    POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:Audio> $<TARGET_RUNTIME_DLLS:EditorApp>
  COMMAND_EXPAND_LISTS
)

add_custom_command(
    TARGET EditorApp
    POST_BUILD
    COMMAND swift build 
    -Xlinker ${CMAKE_BINARY_DIR}/core/engine/audio/Debug/Audio.lib
    -Xlinker ${CMAKE_BINARY_DIR}/core/engine/ecs/Debug/EntityComponentSystem.lib
    -Xlinker ${CMAKE_BINARY_DIR}/core/engine/input/Debug/Input.lib
    -Xlinker ${CMAKE_BINARY_DIR}/core/engine/rendering/Debug/Rendering.lib
    -Xlinker ${CMAKE_BINARY_DIR}/core/editor/assetpipeline/Debug/AssetPipeline.lib
    -Xlinker ${CMAKE_BINARY_DIR}/core/engine/io/Debug/IO.lib
    -Xlinker ${CMAKE_BINARY_DIR}/core/engine/imgui/Debug/ImGui.lib
    --static-swift-stdlib
    COMMENT "Running swift build with custom flags..."
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/.build/debug/KyaniteEditor.dll
            ${CMAKE_BINARY_DIR}/core/editor/app/Debug/KyaniteEditor.dll
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/.build/debug/KyaniteEngine.dll
            ${CMAKE_BINARY_DIR}/core/editor/app/Debug/KyaniteEngine.dll
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/.build/debug/KyaniteEngine.lib
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/KyaniteEngine.lib
COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/.build/debug/KyaniteEditor.lib
			${CMAKE_BINARY_DIR}/core/editor/app/Debug/KyaniteEditor.lib
)