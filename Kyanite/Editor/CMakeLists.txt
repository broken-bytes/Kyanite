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

add_executable(
    Kyanite-Editor
    Editor.cxx
)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
message(STATUS "SDL found")

target_link_libraries(
    Kyanite-Editor
    PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    imgui::imgui
)

target_include_directories(
    Kyanite-Editor
    PUBLIC
    ${SDL_INCLUDE_DIRS}
)


target_include_directories(
    Kyanite-Editor
    PUBLIC
    ./
)


target_include_directories( Kyanite-Editor PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Core/include
)

