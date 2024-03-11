FILE(GLOB_RECURSE HEADERS "include/**.h*")
FILE(GLOB_RECURSE SRC "src/**.c*")

add_library(System SHARED ${SRC} ${HEADERS})

target_include_directories(System PRIVATE include)
target_include_directories(System PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_include_directories(System PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/logger/include)

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(System PUBLIC Logger)
target_link_libraries(System PRIVATE SDL2::SDL2)
