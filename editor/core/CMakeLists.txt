FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.c*")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent)
find_package(unofficial-sqlite3 REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_library(EditorCore SHARED ${SRC} ${HEADERS})

target_include_directories(EditorCore PRIVATE include)
target_include_directories(EditorCore PRIVATE ${CMAKE_SOURCE_DIR}/engine/assetpackages/include)
target_include_directories(EditorCore PRIVATE ${CMAKE_SOURCE_DIR}/engine/engine/include)
target_include_directories(EditorCore PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)
target_include_directories(EditorCore PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)
target_include_directories(EditorCore PRIVATE ${CMAKE_SOURCE_DIR}/engine/rendering/include)

target_link_libraries(EditorCore PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent unofficial::sqlite3::sqlite3 assimp::assimp)

# Link all engine libraries
target_link_libraries(EditorCore PRIVATE Engine Core AssetPackages Rendering)

add_custom_command(TARGET EditorCore POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:EditorCore> $<TARGET_RUNTIME_DLLS:EditorCore>
  COMMAND_EXPAND_LISTS
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${HEADERS})
