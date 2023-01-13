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

find_package(glm CONFIG REQUIRED)
find_package(stduuid CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

add_library(
    Kyanite-Runtime
    STATIC
    src/Engine.cxx
)
target_include_directories(
    Kyanite-Runtime
    PUBLIC
    ${SDL_INCLUDE_DIRS}
)

target_link_libraries(Kyanite-Runtime PUBLIC glm::glm)
target_link_libraries(Kyanite-Runtime PUBLIC stduuid)
target_link_libraries(Kyanite-Runtime PUBLIC flecs::flecs flecs::flecs_static)


target_include_directories(Kyanite-Runtime PUBLIC include/Engine)

target_include_directories(Kyanite-Runtime PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Rendering/include
)

target_include_directories(Kyanite-Runtime PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Core/include
)

