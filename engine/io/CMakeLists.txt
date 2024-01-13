FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB SRC "src/*.cxx")

add_library(IO SHARED ${SRC} ${HEADERS})

target_include_directories(IO PRIVATE include)
target_include_directories(IO PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)
