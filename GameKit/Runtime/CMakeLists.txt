add_executable(App entry.swift Bridge.swift)
target_compile_options(App PRIVATE -enable-experimental-cxx-interop)
target_link_libraries(App PRIVATE GameKit)