cmake_minimum_required(3.24)

project(Kyanite)

# Shared across modules
add_subdirectory(Kyanite/Core)
# Input and Event system
add_subdirectory(Kyanite/Input)
# Physics System
add_subdirectory(Kyanite/Physics)
# Rendering Backends
add_subdirectory(Kyanite/Rendering)
# Engine-Runtime, pre-compiled, loads dynamic lib
add_subdirectory(Kyanite/Runtime)
# Editor code
add_subdirectory(Kyanite/Editor)
