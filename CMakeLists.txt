cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(PROJECT_NAME "Kyanite" LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
project(${PROJECT_NAME})

enable_testing()

add_subdirectory(editor)
add_subdirectory(engine)

add_subdirectory(testapp)
