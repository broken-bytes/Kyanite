# Define Swift source files
file(GLOB_RECURSE SRC "src/*.swift*")

# Set Swift module name
set(SWIFT_MODULE_NAME EditorAssembly)

# Set Swift flags
set(SWIFT_FLAGS -emit-module -module-name ${SWIFT_MODULE_NAME} -emit-library)

# Add a custom command to build the Swift module
add_custom_command(
    OUTPUT ${SWIFT_MODULE_NAME}.swiftmodule ${SWIFT_MODULE_NAME}.swiftdoc
    COMMAND swiftc ${SRC} ${SWIFT_FLAGS} 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/editor/scripting
    DEPENDS ${SWIFT_SOURCES}
)

# Add a custom target
add_custom_target(KyaniteEditorModule ALL DEPENDS ${SWIFT_MODULE_NAME}.swiftmodule)

# Copy the dll to the editor build directory
add_custom_command(
    TARGET KyaniteEditorModule POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:EditorApp>
    ${CMAKE_CURRENT_BINARY_DIR}/${SWIFT_MODULE_NAME}.dll
    COMMENT "Copying ${SWIFT_MODULE_NAME}.dll to $<TARGET_FILE_DIR:EditorApp>"
)
