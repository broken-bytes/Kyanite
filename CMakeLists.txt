cmake_minimum_required(VERSION 3.20)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(Kyanite)

# Shared across modules
#add_subdirectory(Kyanite/Core)
# Utils
add_subdirectory(Kyanite/Utils)
# Input and Event system
#add_subdirectory(Kyanite/Input)
# Physics System
#add_subdirectory(Kyanite/Physics)
# Rendering Backends
add_subdirectory(Kyanite/Rendering)
# Engine-Runtime, pre-compiled, loads dynamic lib
add_subdirectory(Kyanite/Runtime)
# Editor code
#add_subdirectory(Kyanite/Editor)
