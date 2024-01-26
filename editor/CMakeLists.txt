FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.c*")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(unofficial-sqlite3 REQUIRED)
find_package(assimp CONFIG REQUIRED)

qt_standard_project_setup()

qt_add_executable(Editor ${SRC} ${HEADERS})

target_include_directories(Editor PRIVATE include)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/engine/assetpackages/include)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/engine/engine/include)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/engine/rendering/include)

target_link_libraries(Editor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets unofficial::sqlite3::sqlite3 assimp::assimp)


# Link all engine libraries
target_link_libraries(Editor PRIVATE Engine Core AssetPackages Rendering)

add_custom_command(TARGET Editor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:Editor> $<TARGET_RUNTIME_DLLS:Editor>
  COMMAND_EXPAND_LISTS
)

set_target_properties(Editor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${HEADERS})
