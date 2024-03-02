FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB SRC "src/*.cxx")

add_library(Input SHARED ${SRC} ${HEADERS})

target_include_directories(Input PRIVATE include)
target_include_directories(Input PRIVATE ${CMAKE_SOURCE_DIR}/engine/math/include)
target_include_directories(Input PRIVATE ${CMAKE_SOURCE_DIR}/engine/shared/include)
target_include_directories(Input PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)


find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(Input PRIVATE SDL2::SDL2 imgui::imgui)

find_path(ATOMIC_QUEUE_INCLUDE_DIRS "atomic_queue/atomic_queue.h")
target_include_directories(Input PRIVATE ${ATOMIC_QUEUE_INCLUDE_DIRS})
