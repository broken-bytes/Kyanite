FILE(GLOB_RECURSE HEADERS "include/**.h*")
FILE(GLOB_RECURSE SRC "src/**.c*")

add_library(Rendering SHARED ${SRC} ${HEADERS})

target_include_directories(Rendering PRIVATE include)
target_include_directories(Rendering PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_include_directories(Rendering PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/logger/include)
target_include_directories(Rendering PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/math/include)
target_include_directories(Rendering PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/io/include)

find_package(cereal CONFIG REQUIRED)
find_package(FreeImage CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(Rendering PUBLIC Logger)
target_link_libraries(Rendering PRIVATE SDL2::SDL2 spdlog::spdlog glm::glm freeimage::FreeImage freeimage::FreeImagePlus cereal::cereal imgui::imgui)

find_path(ATOMIC_QUEUE_INCLUDE_DIRS "atomic_queue/atomic_queue.h")
target_include_directories(Rendering PRIVATE ${ATOMIC_QUEUE_INCLUDE_DIRS})
