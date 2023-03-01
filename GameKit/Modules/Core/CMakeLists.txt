SET(
    SRC
    src/Core.cxx
)

add_library(Core STATIC ${SRC})

target_include_directories(Core PUBLIC include)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/GameKit/Modules/Core/include)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/GameKit/Modules/Core/include/Core)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/GameKit/Shared/include)

set_target_properties(Core 
    PROPERTIES
    CXX_STANDARD 20
    OUTPUT_NAME Core
)