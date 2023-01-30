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

find_package(plog CONFIG REQUIRED)

set(HEADERS 
    include/Core/Logger.hxx
    )   
set(SRC 
    src/Logger.cxx
)

add_library(Core STATIC ${SRC} ${HEADERS})

target_include_directories(Core PRIVATE include/Core)

target_link_libraries(Core PRIVATE plog::plog)
