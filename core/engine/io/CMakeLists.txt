FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB SRC "src/*.cxx")

find_package(libzip CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(stduuid REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(IO SHARED ${SRC} ${HEADERS})

target_include_directories(IO PRIVATE include)
target_include_directories(IO PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(IO PUBLIC SDL2::SDL2 unofficial-sodium::sodium libzip::zip stduuid glm::glm imgui::imgui)
