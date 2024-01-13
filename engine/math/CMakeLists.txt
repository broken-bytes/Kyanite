FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB SRC "src/*.cxx")

add_library(Math STATIC ${SRC} ${HEADERS})

target_include_directories(Math PRIVATE include/math)
