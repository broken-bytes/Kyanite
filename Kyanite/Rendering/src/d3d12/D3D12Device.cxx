
#include "d3d12/D3D12Device.hxx"
#include "DescriptorHandle.hxx"
#include "ShaderBinding.hxx"
#include "d3d12/D3D12Adapter.hxx"
#include "d3d12/D3D12Allocator.hxx"
#include "d3d12/D3D12Buffer.hxx"
#include "d3d12/D3D12CommandQueue.hxx"
#include "d3d12/D3D12DepthBuffer.hxx"
#include "d3d12/D3D12Fence.hxx"
#include "d3d12/D3D12Frame.hxx"
#include "d3d12/D3D12GraphicsCommandList.hxx"
#include "d3d12/D3D12GraphicsPipelineState.hxx"
#include "d3d12/D3D12GraphicsRootSignature.hxx"
#include "d3d12/D3D12Heap.hxx"
#include "d3d12/D3D12IndexBuffer.hxx"
#include "d3d12/D3D12ReadbackBuffer.hxx"
#include "d3d12/D3D12RenderTarget.hxx"
#include "d3d12/D3D12ShaderBinding.hxx"
#include "d3d12/D3D12SwapChain.hxx"
#include "d3d12/D3D12TextureBuffer.hxx"
#include "d3d12/D3D12UploadBuffer.hxx"
#include "d3d12/D3D12VertexBuffer.hxx"
#include "imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxcapi.h>
#include <dxgiformat.h>
#include <memory>
#include <stdexcept>
#include <wrl.h>
#include <wrl/client.h>


namespace Renderer {
D3D12Device::D3D12Device(HWND window) : Device(window) {
  UINT dxgiFactoryFlags = 0;
#if _DEBUG

  Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
  if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
    debugController->EnableDebugLayer();

    // Enable additional debug layers.
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
  }
#endif

  auto adapter = D3D12Adapter::Adapters()[0];

  if (SUCCEEDED(D3D12CreateDevice(adapter->Raw(), D3D_FEATURE_LEVEL_12_1,
                                  __uuidof(ID3D12Device9), &_device))) {
    OutputDebugStringW(L"Created device \n");
#ifdef _DEBUG
    ID3D12Debug *pdx12Debug = NULL;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
      pdx12Debug->EnableDebugLayer();
    if (pdx12Debug != NULL) {
      ID3D12InfoQueue *pInfoQueue = NULL;
      _device->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
      pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
      pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
      pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
      pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_INFO, true);
      pInfoQueue->Release();
      pdx12Debug->Release();
    }
#endif
  } else {
    throw std::runtime_error("Failed creating device");
  }
}

D3D12Device::~D3D12Device() {}

auto D3D12Device::Name() -> const wchar_t * { return L"Default Device"; }

auto D3D12Device::VRAM() -> const uint32_t { return 0; }

auto D3D12Device::CreateSwapChain(std::shared_ptr<CommandQueue> queue,
                                  uint8_t frameCount, uint32_t width,
                                  uint32_t height)
    -> std::shared_ptr<SwapChain> {
  return std::make_shared<D3D12SwapChain>(
      queue, reinterpret_cast<HWND>(_window), *this, frameCount, width, height);
}

auto D3D12Device::CreateCommandAllocator(CommandType type)
    -> std::shared_ptr<Allocator> {
  Microsoft::WRL::ComPtr<ID3D12CommandAllocator> alloc;
  _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE(type),
                                  IID_PPV_ARGS(&alloc));
  return std::make_shared<D3D12Allocator>(type, alloc);
}

auto D3D12Device::CreateCommandQueue(CommandType type)
    -> std::shared_ptr<CommandQueue> {
  return std::make_shared<D3D12CommandQueue>(type, *this);
}

auto D3D12Device::CreateCommandList(CommandType type,
                                    std::shared_ptr<Allocator> allocator,
                                    const char *name)
    -> std::shared_ptr<GraphicsCommandList> {
  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList5> commandList;
  _device->CreateCommandList(
      0, D3D12_COMMAND_LIST_TYPE(type),
      static_pointer_cast<D3D12Allocator>(allocator)->Native(), nullptr,
      IID_PPV_ARGS(&commandList));
  wchar_t *wstr = new wchar_t[strlen(name)];
  mbstowcs(wstr, name, strlen(name));
  commandList->SetName(wstr);

  delete[] wstr;

  if (commandList == nullptr) {
    throw std::runtime_error("Commandlist is null");
  }

  return std::make_shared<D3D12GraphicsCommandList>(type, commandList);
}
auto D3D12Device::CreateRenderTargetHeap(size_t count)
    -> std::shared_ptr<Heap> {
  return std::make_shared<D3D12Heap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
                                     count);
}
auto D3D12Device::CreateConstantBufferHeap(size_t count)
    -> std::shared_ptr<Heap> {
  return std::make_shared<D3D12Heap>(
      *this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, count, true);
}
auto D3D12Device::CreateShaderResourceHeap(size_t count)
    -> std::shared_ptr<Heap> {
  return std::make_shared<D3D12Heap>(
      *this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, count, true);
}
auto D3D12Device::CreateUnorderedAccessHeap(size_t count)
    -> std::shared_ptr<Heap> {
  return std::make_shared<D3D12Heap>(
      *this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, count);
}
auto D3D12Device::CreateDepthStencilHeap(size_t count)
    -> std::shared_ptr<Heap> {
  return std::make_shared<D3D12Heap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                                     count);
}
auto D3D12Device::CreateSamplerHeap(size_t count) -> std::shared_ptr<Heap> {
  return std::make_shared<D3D12Heap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
                                     count);
}
auto D3D12Device::CreateVertexBuffer(std::vector<Vertex> vertices, std::string name)
    -> std::shared_ptr<Renderer::Buffer> {
  auto len = sizeof(Vertex);
  const uint32_t vertexBufferSize = vertices.size() * sizeof(Vertex);
  auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
  auto resourceDesc =
      CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize, D3D12_RESOURCE_FLAG_NONE);

  Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
  Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
  _device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
                                   &resourceDesc, D3D12_RESOURCE_STATE_COMMON,
                                   nullptr, IID_PPV_ARGS(&buffer));

  D3D12_VERTEX_BUFFER_VIEW view = {};
  view.BufferLocation = buffer->GetGPUVirtualAddress();
  view.StrideInBytes = sizeof(Vertex);
  view.SizeInBytes = vertexBufferSize;

  return std::make_shared<D3D12Buffer<D3D12_VERTEX_BUFFER_VIEW>>(
      buffer, view, vertices.size(), name);
}

auto D3D12Device::CreateIndexBuffer(std::vector<uint32_t> indices, std::string name)
    -> std::shared_ptr<Renderer::Buffer> {
  const uint32_t indexBufferSize = indices.size() * sizeof(uint32_t);
  auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
  auto resourceDesc =
      CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize, D3D12_RESOURCE_FLAG_NONE);

  Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
  Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
  _device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
                                   &resourceDesc, D3D12_RESOURCE_STATE_COMMON,
                                   nullptr, IID_PPV_ARGS(&buffer));

  D3D12_INDEX_BUFFER_VIEW view = {};
  view.BufferLocation = buffer->GetGPUVirtualAddress();
  view.Format = DXGI_FORMAT_R32_UINT;
  view.SizeInBytes = indexBufferSize;

  return std::make_shared<D3D12Buffer<D3D12_INDEX_BUFFER_VIEW>>(buffer, view,
                                                                indices.size(), name);
}

auto D3D12Device::CreateUploadBuffer(size_t sizeInBytes, std::string name)
    -> std::shared_ptr<Renderer::UploadBuffer> {
  Microsoft::WRL::ComPtr<ID3D12Resource> buffer;

  auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
  auto resourceDesc =
      CD3DX12_RESOURCE_DESC::Buffer(sizeInBytes, D3D12_RESOURCE_FLAG_NONE);

  _device->CreateCommittedResource(
      &heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
      D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer));

  auto uBuffer = std::make_shared<D3D12UploadBuffer>(buffer, name);
  uBuffer->Count = sizeInBytes;

  return uBuffer;
}

auto D3D12Device::CreateVertexArrayObject(Renderer::VertexBuffer *vertexBuffer,
                                          Renderer::IndexBuffer *indexBuffer)
    -> std::shared_ptr<VertexArrayObject> {
  return nullptr;
}
auto D3D12Device::CreateCommandQueue() -> std::shared_ptr<CommandQueue> {
  return nullptr;
}
auto D3D12Device::CreateTextureBuffer(uint16_t width, uint16_t height,
                                      uint8_t channels, uint8_t mipLevels,
                                      TextureFormat format, std::string name)
    -> std::shared_ptr<Renderer::TextureBuffer> {
  Microsoft::WRL::ComPtr<ID3D12Resource> buffer;

  DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;

  D3D12_CLEAR_VALUE clear;


  switch(format) {
    case RGBA:
    dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    clear = { dxgiFormat, 1, 1, 1, 1};
    break;
    case RGBA_UINT:
    dxgiFormat = DXGI_FORMAT_R8G8B8A8_UINT;
    clear = { dxgiFormat, 255, 255, 255, 255};
    break;
  }

  auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
  D3D12_RESOURCE_DESC textureDesc = {};
  textureDesc.MipLevels = mipLevels;
  textureDesc.Format = dxgiFormat;
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
  textureDesc.DepthOrArraySize = 1;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;


  _device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
                                   &textureDesc, D3D12_RESOURCE_STATE_COMMON,
                                   &clear, IID_PPV_ARGS(&buffer));

  return std::make_shared<D3D12TextureBuffer>(buffer, name);
}

auto D3D12Device::CreateReadbackBuffer(size_t sizeInBytes)
    -> std::shared_ptr<ReadbackBuffer> {
  D3D12_HEAP_PROPERTIES readbackHeapProperties{
      CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK)};
  Microsoft::WRL::ComPtr<ID3D12Resource> readbackBuffer;

  D3D12_RESOURCE_DESC bufferDesc = {};
        bufferDesc.DepthOrArraySize = 1;
        bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
        bufferDesc.Height = 1;
        bufferDesc.Width = ((sizeInBytes) + 255) & ~255;
        bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        bufferDesc.MipLevels = 1;
        bufferDesc.SampleDesc.Count = 1;
  _device->CreateCommittedResource(
      &readbackHeapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc,
      D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&readbackBuffer));

	return std::make_shared<D3D12ReadbackBuffer>(readbackBuffer, ((sizeInBytes) + 255) & ~255);
}

auto D3D12Device::CreateConstantBuffer(void *data, size_t size,
                                       const char *name)
    -> std::shared_ptr<Buffer> {
  return nullptr;
}

auto D3D12Device::CreateGraphicsRootSignature(
    GraphicsRootSignatureDescription &rootDesc)
    -> std::shared_ptr<GraphicsRootSignature> {
  return std::make_shared<D3D12GraphicsRootSignature>(*this, rootDesc);
}

auto D3D12Device::CreatePipelineState(
    std::shared_ptr<GraphicsRootSignature> rootSignature,
    std::vector<GraphicsPipelineInputElement> inputLayout,
    std::shared_ptr<GraphicsShaderBinding> shaderBinding,
    TextureFormat format,
    bool depth,
    bool stencil, GraphicsPipelineStateTopology topology)
    -> std::shared_ptr<GraphicsPipelineState> {
  return std::make_shared<D3D12GraphicsPipelineState>(
      *this, rootSignature, inputLayout, shaderBinding, format, 0, topology);
}

auto D3D12Device::CreateDepthBuffer(glm::vec2 dimensions, std::string name)
    -> std::shared_ptr<Buffer> {
  D3D12_CLEAR_VALUE optimizedClearValue = {};
  optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
  optimizedClearValue.DepthStencil = {1.0f, 0};

  auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
  auto desc = CD3DX12_RESOURCE_DESC::Tex2D(
      DXGI_FORMAT_D32_FLOAT, dimensions.x, dimensions.y, 1, 0, 1, 0,
      D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

  Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
  D3D12_DEPTH_STENCIL_VIEW_DESC view;
  view.Format = DXGI_FORMAT_D32_FLOAT;
  view.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
  view.Texture2D.MipSlice = 0;
  view.Flags = D3D12_DSV_FLAG_NONE;

  if (S_OK != _device->CreateCommittedResource(
                  &props, D3D12_HEAP_FLAG_NONE, &desc,
                  D3D12_RESOURCE_STATE_DEPTH_WRITE, &optimizedClearValue,
                  IID_PPV_ARGS(&buffer))) {
    throw std::runtime_error("Failed to create Depth Buffer");
  }
  return std::make_shared<D3D12Buffer<D3D12_DEPTH_STENCIL_VIEW_DESC>>(buffer,
                                                                      view, 0, name);
}

auto D3D12Device::CreateDepthStencilView(
    std::shared_ptr<DescriptorHandle> handle, std::shared_ptr<Buffer> buffer)
    -> void {
  auto dBuff =
      static_pointer_cast<D3D12Buffer<D3D12_DEPTH_STENCIL_VIEW_DESC>>(buffer);
  auto view = dBuff->View;
  _device->CreateDepthStencilView(
      dBuff->Buffer.Get(), &view,
      static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(
          handle)
          ->Handle());
}
auto D3D12Device::CreateFrame(std::shared_ptr<Allocator> allocator,
                              std::shared_ptr<RenderTarget> renderTarget)
    -> std::shared_ptr<Frame> {
  return std::make_shared<D3D12Frame>(allocator, renderTarget);
}

auto D3D12Device::CreateFence(std::uint64_t fenceValue)
    -> std::shared_ptr<Fence> {
  return std::make_shared<D3D12Fence>(*this, fenceValue);
}

auto D3D12Device::CreateRenderTarget(std::shared_ptr<TextureBuffer> texture, std::string name)
    -> std::shared_ptr<RenderTarget> {
  auto buff = static_pointer_cast<D3D12TextureBuffer>(texture)->Raw();
  return std::make_shared<D3D12RenderTarget>(buff, name);
}

auto D3D12Device::CreateRenderTargetView(
    std::shared_ptr<DescriptorHandle> handle,
    std::shared_ptr<RenderTarget> target) -> std::shared_ptr<RenderTarget> {
  _device->CreateRenderTargetView(
      static_pointer_cast<D3D12RenderTarget>(target)->Resource(), nullptr,
      static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(
          handle)
          ->Handle());
  return nullptr;
}

auto D3D12Device::CreateShaderResourceView(
    std::shared_ptr<TextureBuffer> buffer,
    std::shared_ptr<DescriptorHandle> handle) -> void {
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;

  _device->CreateShaderResourceView(
      static_pointer_cast<D3D12TextureBuffer>(buffer)->Raw(), &srvDesc,
      static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(
          handle)
          ->Handle());
}

auto D3D12Device::CreateConstantBufferView(
    std::shared_ptr<Heap> heap, std::shared_ptr<UploadBuffer> buffer,
    std::shared_ptr<DescriptorHandle> cpuHandle) -> void {
  D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
  auto dBuffer = static_pointer_cast<D3D12UploadBuffer>(buffer);
  desc.SizeInBytes = buffer->Size();
  desc.BufferLocation = dBuffer->Buffer->GetGPUVirtualAddress();

  auto cHandle =
      static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(
          cpuHandle);

  _device->CreateConstantBufferView(&desc, cHandle->Handle());
}

auto D3D12Device::CreateMipMaps(
    std::shared_ptr<TextureBuffer> texture,
    std::shared_ptr<GraphicsCommandList> commandList, uint16_t width,
    uint16_t height, uint8_t levels) -> void {}

auto D3D12Device::CompileShader(std::string code)
    -> std::shared_ptr<GraphicsShaderBinding> {
  Microsoft::WRL::ComPtr<IDxcUtils> utils;
  Microsoft::WRL::ComPtr<IDxcCompiler3> compiler;
  DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
  DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
  auto binding = new D3D12GraphicsShaderBinding();

  size_t vertLen;
  size_t pixLen;
  ID3DBlob *err;
  Microsoft::WRL::ComPtr<IDxcIncludeHandler> pIncludeHandler;
  utils->CreateDefaultIncludeHandler(&pIncludeHandler);

  Microsoft::WRL::ComPtr<IDxcResult> vsResults;
  Microsoft::WRL::ComPtr<IDxcResult> psResults;

  {
    IDxcOperationResult *operationResult;
    LPCWSTR args[] = {
        L"Unnamed", L"-E",     L"VSMain", // Entry point.
        L"-T",      L"vs_6_6",            // Target.
        L"-Zs", // Enable debug information (slim format)
    };

    Microsoft::WRL::ComPtr<IDxcBlobEncoding> rawSource = nullptr;

    DxcBuffer source;
    source.Ptr = code.c_str();
    source.Size = code.size();
    source.Encoding = DXC_CP_ACP; // Assume BOM says UTF8 or U
    compiler->Compile(&source, args, _countof(args), pIncludeHandler.Get(),
                      IID_PPV_ARGS(&vsResults));

    Microsoft::WRL::ComPtr<IDxcBlobUtf8> pErrors = nullptr;
    vsResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);

    if (pErrors != nullptr && pErrors->GetStringLength() != 0) {
      OutputDebugString(pErrors->GetStringPointer());
    }

    HRESULT hrStatus;
    vsResults->GetStatus(&hrStatus);
    if (FAILED(hrStatus)) {
      throw std::runtime_error("Failed to compile shader file");
    }

    vsResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&binding->VertexShader),
                         nullptr);
  }

  {
    IDxcOperationResult *operationResult;
    LPCWSTR args[] = {
        L"Unnamed", L"-E",     L"PSMain", // Entry point.
        L"-T",      L"ps_6_0",            // Target.
        L"-Zs", // Enable debug information (slim format)
    };

    DxcBuffer source;
    source.Ptr = code.c_str();
    source.Size = code.size();
    source.Encoding = DXC_CP_ACP; // Assume BOM says UTF8 or U

    compiler->Compile(&source, args, _countof(args), pIncludeHandler.Get(),
                      IID_PPV_ARGS(&psResults));
    Microsoft::WRL::ComPtr<IDxcBlobUtf8> pErrors = nullptr;
    psResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);
    if (pErrors != nullptr && pErrors->GetStringLength() != 0) {
      OutputDebugString(pErrors->GetStringPointer());
    }

    HRESULT hrStatus;
    psResults->GetStatus(&hrStatus);
    if (FAILED(hrStatus)) {
      throw std::runtime_error("Failed to compile shader file");
    }

    psResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&binding->PixelShader),
                         nullptr);
  }

  return std::shared_ptr<D3D12GraphicsShaderBinding>(binding);
}

auto D3D12Device::Flush() -> void {}

auto D3D12Device::Native() -> ID3D12Device4 * { return _device.Get(); }
} // namespace Renderer