FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.c*")

find_package(imgui CONFIG REQUIRED)

add_library(ImGui SHARED ${SRC} ${HEADERS})

target_include_directories(ImGui PRIVATE include)
target_include_directories(ImGui PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_link_libraries(ImGui PRIVATE imgui::imgui)

add_custom_command(TARGET ImGui POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ImGui> $<TARGET_RUNTIME_DLLS:ImGui>
  COMMAND_EXPAND_LISTS
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${HEADERS})
