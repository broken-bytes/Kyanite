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

add_library(
    Physics
    src/PhysicsHandler.cxx
    include/Physics/PhysicsHandler.hxx
)

target_include_directories(Physics PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Physics/include
)

target_link_directories(
    Physics
    PUBLIC
    ${CMAKE_SOURCE_DIR}/external/PhysX/lib/win64/debug
)

target_include_directories(
    Physics
    PUBLIC
    ${CMAKE_SOURCE_DIR}/external/PhysX/include
)

target_link_libraries(
    Physics
    PUBLIC
    PhysXCommon_64.lib
    PhysX_64.lib
    PhysXFoundation_64.lib
    PhysXCooking_64.lib
    glm::glm
)

