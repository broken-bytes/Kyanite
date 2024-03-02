FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.cxx")

add_library(Logger SHARED ${SRC} ${HEADERS})

target_include_directories(Logger PRIVATE include)
target_include_directories(Logger PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)

find_package(cereal CONFIG REQUIRED)

target_link_libraries(Logger PRIVATE cereal::cereal)
