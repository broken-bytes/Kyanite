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


set(HEADERS
    include/Rendering/Renderer.hxx
)

if(WIN32)
SET(SRC
    src/d3d12/D3D12Renderer.cxx
)
endif()

add_library(
    Rendering
    ${SRC}
    ${HEADERS}
)

find_package(SDL2 CONFIG REQUIRED)
message(STATUS "SDL found")

target_link_libraries(
    Rendering
    PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
)

target_include_directories(
    Rendering
    PUBLIC
    ${SDL_INCLUDE_DIRS}
)

target_include_directories(
    Rendering
    PRIVATE
    include/Rendering
)



