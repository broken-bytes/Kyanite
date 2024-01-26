FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB SRC "src/*.cxx")

find_package(libzip CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(stduuid REQUIRED)

add_library(Core SHARED ${SRC} ${HEADERS})

target_include_directories(Core PRIVATE include)
target_include_directories(Core PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Core PUBLIC SDL2::SDL2 unofficial-sodium::sodium libzip::zip stduuid)
