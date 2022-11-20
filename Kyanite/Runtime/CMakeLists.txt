cmake_minimum_required(VERSION 3.20)

add_executable(
    Kyanite
    Main.cxx
)

target_include_directories(Kyanite PUBLIC ${SDL2_INCLUDE_DIRS})

if(WIN32)
target_link_directories(Kyanite PUBLIC ${SDL2_LIBRARY_DIRS})
target_link_libraries(
    Kyanite
    PRIVATE
    SDL2
    SDL2main
)
# Add Renderdoc on Windows so we can easily debug rendering
target_include_directories(Kyanite PRIVATE ${Renderdoc_DIR})
else()
find_package(SDL2 REQUIRED)
target_link_libraries(
    Kyanite
    PUBLIC
    SDL2::SDL2
)

target_include_directories(
    Kyanite
    PUBLIC
    ${SDL_INCLUDE_DIRS}
)
endif()

# For now directly include Core, later will be moved to dll loading
target_include_directories(Kyanite PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Core/include
)

# For now directly include Rendering, later will be moved to dll loading
target_include_directories(Kyanite PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Rendering/include
)
