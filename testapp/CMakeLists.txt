SET(
    SRC
    main.cxx
)

add_executable(TestApp WIN32 ${SRC})

target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/engine/audio/include)
target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)
target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/engine/input/include)
target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/engine/rendering/include)
target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)
target_link_libraries(TestApp PRIVATE Audio Core Input Rendering)

add_custom_command(TARGET TestApp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:TestApp> $<TARGET_RUNTIME_DLLS:TestApp>
  COMMAND_EXPAND_LISTS
)