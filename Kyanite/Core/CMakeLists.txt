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

find_package(freeimage CONFIG REQUIRED)
find_package(jsoncons CONFIG REQUIRED)
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

set(HEADERS 
    include/Core/AssetLoader.hxx
    include/Core/VirtualFileSystem.hxx
    )   
set(SRC 
    src/AssetLoader.cxx
    src/VirtualFileSystem.cxx
)

add_library(Core STATIC ${SRC} ${HEADERS})

target_include_directories(Core PRIVATE ${TINYGLTF_INCLUDE_DIRS})
target_include_directories(Core PRIVATE include/Core)

target_link_libraries(Core PRIVATE freeimage::FreeImage freeimage::FreeImagePlus)
target_link_libraries(Core PRIVATE jsoncons)

