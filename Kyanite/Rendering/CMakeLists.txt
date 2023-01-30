cmake_minimum_required(VERSION 3.20)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS 
    include/Rendering/Allocator.hxx
    include/Rendering//Buffer.hxx
    include/Rendering/CommandQueue.hxx
    include/Rendering/DescriptorHandle.hxx
    include/Rendering/Device.hxx
    include/Rendering/DirectionalLight.hxx
    include/Rendering/Fence.hxx
    include/Rendering/Frame.hxx
    include/Rendering/GPUResource.hxx
    include/Rendering/GraphicsCommandList.hxx
    include/Rendering/GraphicsPipelineState.hxx
    include/Rendering/GraphicsRootSignature.hxx
    include/Rendering/Heap.hxx
    include/Rendering/IndexBuffer.hxx
    include/Rendering/Interface.hxx
    include/Rendering/Material.hxx
    include/Rendering/MeshData.hxx
    include/Rendering/PointLight.hxx
    include/Rendering/Rect.hxx
    include/Rendering/Resource.hxx
    include/Rendering/RenderTarget.hxx
    include/Rendering/SceneData.hxx
    include/Rendering/Shader.hxx
    include/Rendering/ShaderBinding.hxx
    include/Rendering/ShaderData.hxx
    include/Rendering/SwapChain.hxx
    include/Rendering/ReadbackBuffer.hxx
    include/Rendering/TextureBuffer.hxx
    include/Rendering/Vertex.hxx
    include/Rendering/UploadBuffer.hxx
    include/Rendering/VertexArrayObject.hxx
    include/Rendering/VertexBuffer.hxx
    include/Rendering/Viewport.hxx
)   
set(SRC 
    src/Interface.cxx
)

if(WIN32)
list(APPEND HEADERS
    internal/d3d12/D3D12Allocator.hxx
    internal/d3d12/D3D12Renderer.hxx
    internal/d3d12/D3D12Adapter.hxx
    internal/d3d12/D3D12Buffer.hxx
    internal/d3d12/D3D12CommandQueue.hxx
    internal/d3d12/D3D12Device.hxx
    internal/d3d12/D3D12DepthBuffer.hxx
    internal/d3d12/D3D12Fence.hxx
    internal/d3d12/D3D12Frame.hxx
    internal/d3d12/D3D12GraphicsCommandList.hxx
    internal/d3d12/D3D12GraphicsPipelineState.hxx
    internal/d3d12/D3D12GraphicsRootSignature.hxx
    internal/d3d12/D3D12Heap.hxx
    internal/d3d12/D3D12IndexBuffer.hxx
    internal/d3d12/D3D12RenderTarget.hxx
    internal/d3d12/D3D12Object.hxx
    internal/d3d12/D3D12Resource.hxx
    internal/d3d12/D3D12Shader.hxx
    internal/d3d12/D3D12ShaderBinding.hxx
    internal/d3d12/D3D12SwapChain.hxx
    internal/d3d12/D3D12ReadbackBuffer.hxx
    internal/d3d12/D3D12TextureBuffer.hxx
    internal/d3d12/D3D12UploadBuffer.hxx
    internal/d3d12/D3D12VertexBuffer.hxx
    internal/d3d12/Utils.hxx

)
list(APPEND SRC 
    src/d3d12/D3D12Allocator.cxx
    src/d3d12/D3D12Adapter.cxx
    src/d3d12/D3D12CommandQueue.cxx
    src/d3d12/D3D12Device.cxx
    src/d3d12/D3D12DepthBuffer.cxx
    src/d3d12/D3D12Fence.cxx
    src/d3d12/D3D12Frame.cxx
    src/d3d12/D3D12GraphicsCommandList.cxx
    src/d3d12/D3D12GraphicsPipelineState.cxx
    src/d3d12/D3D12GraphicsRootSignature.cxx
    src/d3d12/D3D12Heap.cxx
    src/d3d12/D3D12IndexBuffer.cxx
    src/d3d12/D3D12Shader.cxx
    src/d3d12/D3D12SwapChain.cxx
    src/d3d12/D3D12ReadbackBuffer.cxx
    src/d3d12/D3D12TextureBuffer.cxx
    src/d3d12/D3D12VertexBuffer.cxx
)
endif()

find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(
    Rendering
    ${SRC}
    ${HEADERS}
)

find_package(SDL2 CONFIG REQUIRED)
message(STATUS "SDL found")

target_link_libraries(
    Rendering
    PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
)

target_include_directories(
    Rendering
    PUBLIC
    ${SDL_INCLUDE_DIRS}
)


target_include_directories(Rendering PUBLIC glm::glm)
target_include_directories(Rendering PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Core/include
)
target_include_directories(Rendering PUBLIC include/Rendering)
target_include_directories(Rendering PUBLIC internal)

target_link_libraries(
    Rendering
    PUBLIC
    glm::glm
)

message(STATUS glm::glm)


if(WIN32)
find_path(D3DX12_INCLUDE_DIRS "d3dx12.h")
find_package(directx-dxc CONFIG REQUIRED)
find_package(directx-headers CONFIG REQUIRED)

target_include_directories(Rendering PRIVATE ${D3DX12_INCLUDE_DIRS})
target_include_directories(Rendering PRIVATE
  ${CMAKE_SOURCE_DIR}/Kyanite/Core/include
)


target_link_libraries(
    Rendering
    PRIVATE
    d3d12.lib
    dxgi.lib
    dxguid.lib
    dxcompiler.lib
    Microsoft::DirectXShaderCompiler
    Microsoft::DirectX-Guids
    Microsoft::DirectX-Headers
    imgui::imgui
)
endif()



