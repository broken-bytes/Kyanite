cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(GameKit LANGUAGES CXX Swift)

include(cmake/dependencies/dxheaders.cmake)
#include(cmake/dependencies/flecs.cmake)
include(cmake/dependencies/freeimage.cmake)
include(cmake/dependencies/physx.cmake)
include(cmake/dependencies/sdl.cmake)
#include(cmake/dependencies/tinygltf.cmake)

add_subdirectory(GameKit)

