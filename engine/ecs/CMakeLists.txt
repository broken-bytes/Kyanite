FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB SRC "src/*.c*")

add_library(ECS SHARED ${SRC} ${HEADERS})

target_include_directories(ECS PRIVATE include)
target_include_directories(ECS PRIVATE ${CMAKE_SOURCE_DIR}/engine/core/include)
target_include_directories(ECS PRIVATE ${CMAKE_SOURCE_DIR}/engine/math/include)
target_include_directories(ECS PRIVATE ${CMAKE_SOURCE_DIR}/engine/rendering/include)

find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Flecs CONFIG REQUIRED)

target_link_libraries(ECS PRIVATE spdlog::spdlog glm::glm flecs::flecs_static Rendering)

find_path(ATOMIC_QUEUE_INCLUDE_DIRS "atomic_queue/atomic_queue.h")
target_include_directories(ECS PRIVATE ${ATOMIC_QUEUE_INCLUDE_DIRS})
