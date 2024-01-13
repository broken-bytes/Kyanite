FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB SRC "src/*.cxx")

add_library(Core SHARED ${SRC} ${HEADERS})

target_include_directories(Core PRIVATE include)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Core PRIVATE SDL2::SDL2)