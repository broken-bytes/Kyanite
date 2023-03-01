SET(
    SRC
    src/Renderer.cxx
)

if(WIN32)
    SET(
        SRC_PLATFORM
        src/d3d12/D3D12Adapter.cxx
        src/d3d12/D3D12Device.cxx
    )
endif()

add_library(Rendering STATIC ${SRC} ${SRC_PLATFORM})

target_include_directories(Rendering PUBLIC include)
target_include_directories(Rendering PUBLIC ${CMAKE_SOURCE_DIR}/GameKit/Shared/include)
target_include_directories(Rendering PUBLIC ${CMAKE_SOURCE_DIR}/GameKit/Modules/Math/include)


set_target_properties(Rendering 
    PROPERTIES
    CXX_STANDARD 20
    OUTPUT_NAME Rendering
)

if(WIN32)
    target_link_libraries(Rendering PUBLIC DirectX-Headers)
    target_link_libraries(
        Rendering
        PRIVATE
        d3d12.lib
        dxgi.lib
        dxguid.lib
        dxcompiler.lib
    )
endif()

set(
    SWIFT_SRC
    Sources/Renderer.swift
    Sources/Buffer.swift
)

