#include "Interface.hxx"
#include "DescriptorHandle.hxx"
#include "Mesh.hxx"
#include "Rect.hxx"
#include "Vertex.hxx"
#include "Viewport.hxx"

#include "d3d12/D3D12GraphicsCommandList.hxx"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "glm/vec4.hpp"
#include <SDL_syswm.h>
#include <d3d12.h>
#include <dxgiformat.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"

#if _WIN32
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

#endif

#ifdef __APPLE__
#include "backends/opengl/OpenGLDevice.hxx"
#endif // __APPLE__

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NO_MIN_MAX
#include "d3d12/D3D12Device.hxx"
#include <Windows.h>

#endif
#include <d3dx12.h>

#include <clocale>

#include "d3d12/D3D12Heap.hxx"

float xPos = 1;
float yPos = 1;
float zPos = 1;
glm::vec4 sunColor = {0, 0.1f, 0.4f, 1};
glm::vec4 ambientColor = {1, 1, 1, 1};
float ambientIntensity = 1;

glm::vec3 camPos;
glm::vec3 camRot;

namespace Renderer {
struct MVPCBuffer {
  glm::mat4 Model;
  glm::mat4 ViewProjection;
};

struct MaterialBuffer {
  float Specular;
  float Emmisive;
};

struct SunLight {
  glm::vec4 Color;
  glm::vec4 Position;
  bool Active;
  uint8_t Padding[15];
};

struct AmbientLight {
  glm::vec4 Color;
  float Intensity;
  uint8_t Padding[12];
};

struct PointLight {
  glm::vec4 Color;
  glm::vec4 Position;
  float Radius;
  float Intensity;
  bool Active;
  uint8_t Padding[7];
};

struct LightBuffer {
  SunLight Sun;
  AmbientLight Ambient;
  PointLight Lights[16];
};

auto light = LightBuffer{};

Interface::Interface(uint32_t width, uint32_t height, void *window,
                     RenderBackendAPI type) {
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(reinterpret_cast<SDL_Window *>(window), &wmInfo);
  _window = window;
  HWND hwnd = wmInfo.info.win.window;
  _windowDimension = {(float)width, (float)height};
  switch (type) {
  case DirectX12:
    _device = static_pointer_cast<Device>(std::make_shared<D3D12Device>(hwnd));
    break;
#ifdef __APPLE__
  case OpenGL:
    _device = std::make_shared<OpenGLDevice>(width, height, window);
    break;
#endif // __APPLE__
  default:
    _device = static_pointer_cast<Device>(
        std::make_shared<D3D12Device>(reinterpret_cast<HWND>(window)));
    break;
  }
  setlocale(LC_ALL, "en_US.utf8");

  CreatePipeline();
}

Interface::~Interface() {
  ImGui_ImplDX12_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

auto Interface::StartFrame() -> void {
#if _WIN32

  ImGui_ImplWin32_NewFrame();
  ImGui_ImplDX12_NewFrame();
#endif
  ImGui::NewFrame();
  // Start Projection
  _uploadFenceValue++;
  _uploadCommandList->Close();
  _mainQueue->ExecuteCommandLists({_uploadCommandList});

  auto _ = _mainQueue->Signal(_uploadFence, _uploadFenceValue);

  _uploadFence->Wait(_uploadFenceValue, _uploadFenceEvent);

  _uploadBuffers.clear();

  _computeFenceValue++;

  _computeCommandList->Close();
  _computeQueue->ExecuteCommandLists({_computeCommandList});

  _ = _computeQueue->Signal(_computeFence, _computeFenceValue);

  _uploadFence->Wait(_computeFenceValue, _computeFenceEvent);

  auto fenceValue = _frames[_frameIndex]->FenceValue();

  _frames[_frameIndex]->Allocator()->Reset();
  _commandList->Reset(_frames[_frameIndex]->Allocator(), nullptr);
  _cbAllocator[_frameIndex]->Reset();
  _cbCommandList->Reset(_cbAllocator[_frameIndex], nullptr);
}

auto Interface::MidFrame() -> void {
  auto rtvHandle = _rtvHeap->CpuHandleFor(_frameIndex);
  auto dsvHandle = _dsvHeap->CpuHandleFor(0);
  _commandList->SetRenderTarget(rtvHandle, dsvHandle);

  _commandList->SetDescriptorHeaps({_srvHeap});

  ImGui::Begin("Sun Light");

  ImGui::End();

  auto sun = SunLight{sunColor, {xPos, yPos, zPos, 1}, true};

  light.Ambient = {ambientColor, ambientIntensity};

  light.Sun = sun;

  ImGui::Begin("Lights");
  ImGui::PushID(0);
  if (ImGui::CollapsingHeader("Sun")) {
    ImGui::SliderFloat("X", &xPos, -50.0f, 50.0f);
    ImGui::SliderFloat("Y", &yPos, -100.0f, 200.0f);
    ImGui::SliderFloat("Z", &zPos, -50.0f, 50.0f);
    ImGui::ColorEdit4("Sun Color", (float *)&sunColor);
  }
  ImGui::PopID();

  ImGui::PushID(1);
  if (ImGui::CollapsingHeader("Ambient")) {
    ImGui::ColorEdit4("Ambient Color", (float *)&ambientColor);
    ImGui::SliderFloat("Intensity", &ambientIntensity, 0.0f, 10.0f);
  }
  ImGui::PopID();

  for (int x = 0; x < 16; x++) {
    std::stringstream ss;
    ss << "Point Lights " << x;

    ImGui::PushID(x + 2);
    if (ImGui::CollapsingHeader(ss.str().c_str())) {
      ImGui::Checkbox("Active", &light.Lights[x].Active);
      ImGui::SliderFloat("X", &light.Lights[x].Position.x, -50.0f, 50.0f);
      ImGui::SliderFloat("Y", &light.Lights[x].Position.y, -100.0f, 200.0f);
      ImGui::SliderFloat("Z", &light.Lights[x].Position.z, -50.0f, 50.0f);
      ImGui::ColorEdit4("Color", (float *)&light.Lights[x].Color);
      ImGui::SliderFloat("Intensity", &light.Lights[x].Intensity, 0.0f, 10.0f);
      ImGui::SliderFloat("Range", &light.Lights[x].Radius, 0.0f, 25.0f);
    }
    ImGui::PopID();
  }
  ImGui::End();
  _commandList->SetGraphicsRootConstantBuffer(
      _lightDataBuffer[_frameIndex], &light, sizeof(light),
      _lightDataGPUAddress[_frameIndex]);

  _commandList->Transition(_frames[_frameIndex]->RenderTarget(),
                           ResourceState::PRESENT,
                           ResourceState::RENDER_TARGET);

  // Record commands.
  glm::vec4 clearColor = {0, 0.1f, 0.4f, 1};
  _commandList->ClearRenderTarget(rtvHandle, clearColor);
  _commandList->ClearDepthTarget(dsvHandle);
  _commandList->SetTopology(GraphicsPipelineStateTopology::POLYGON);
  _commandList->SetViewport(_viewport);
  _commandList->SetScissorRect(_scissorRect);

  glm::vec3 cameraPos = {camPos.x, camPos.y, camPos.z -20};
  glm::vec3 cameraFront = {0.0f, 0.0f, 1.0f};
  glm::vec3 cameraLookAt = camPos;
  glm::vec3 cameraUp = {0.0f, 1.0f, 0.0f};

  _viewMatrix = glm::lookAtLH(cameraPos, cameraLookAt, cameraUp);
  //_viewMatrix = glm::inverse(_viewMatrix);
  _projectionMatrix =
      glm::perspectiveFovLH(glm::radians(55.0f), (float)_windowDimension.x,
                            (float)_windowDimension.y, 0.01f, 1000.0f);

  for (auto &mesh : _meshesToDraw) {
    // Update the projection matrix.
    float aspectRatio =
        _windowDimension.x / static_cast<float>(_windowDimension.y);
    // Update the MVP matrix

    glm::mat4 modelView = mesh.Transform;
    modelView = glm::scale(modelView, {1, 1, 1});
    modelView = glm::translate(modelView, {0, -6.0f, 4});
    modelView = glm::rotate(modelView, glm::radians(-130.0f),
                            glm::vec3{1.0f, 0.0f, 0.0f});
    //

    auto mvp = MVPCBuffer{modelView, _projectionMatrix * _viewMatrix};

    auto material = _materials[mesh.MaterialId];
    auto shader = material->Shader;


    _commandList->SetGraphicsRootSignature(shader->Pipeline->RootSignature());
    _commandList->SetPipelineState(shader->Pipeline);
    _commandList->SetGraphicsRootConstants(0, sizeof(mvp) / 4, &mvp, 0);                  // b0     -> The Model View Projection for this Mesh/Material
    _commandList->SetGraphicsRootDescriptorTable(1, _srvHeap->GpuHandleFor(_frameIndex)); // b1     -> The Constant Buffer for this Mesh/Material

    for (auto [it, end, x] = std::tuple{material->Textures.cbegin(), material->Textures.cend(), 0}; it != end; it++, x++){
       _commandList->SetGraphicsRootDescriptorTable(x + 2, it->second->GPUHandle);
    }

    // 2x Table = 2 | 2
    // 2x SRV = 4   | 4 + 2 = 6
    //

    auto len = sizeof(float);

    auto vao = _meshes[mesh.MeshId];
    _commandList->SetVertexBuffer(vao->VertexBuffer);
    _commandList->SetIndexBuffer(vao->IndexBuffer);
    _commandList->DrawInstanced(vao->IndexBuffer->Size(), 1, 0, 0, 0);
  }

  ImGui::Render();
#if _WIN32
  auto cmdList =
      static_pointer_cast<D3D12GraphicsCommandList>(_commandList)->Native();
  ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
#endif

  // Indicate that the back buffer will now be used to present.
  _commandList->Transition(_frames[_frameIndex]->RenderTarget(),
                           ResourceState::RENDER_TARGET,
                           ResourceState::PRESENT);

  _commandList->Close();
  _cbCommandList->Close();

  _mainQueue->ExecuteCommandLists({_commandList});

  // Present the frame.
  _swapChain->Swap();
  _meshesToDraw.clear();
}

auto Interface::EndFrame() -> void {
  MoveToNextFrame();
  _device->Flush();
  _uploadAllocator->Reset();
  _uploadCommandList->Reset(_uploadAllocator, nullptr);
  _computeAllocator->Reset();
  _computeCommandList->Reset(_computeAllocator, nullptr);
}

auto Interface::Update() -> void {}

auto Interface::Resize(uint32_t width, uint32_t height) -> void {}

auto Interface::CreateMaterial(uint64_t shaderId) -> uint64_t {
  auto mat = std::make_shared<Material>();
  mat->Shader = _shaders[shaderId];

  _materials.push_back(mat);

  return _materials.size() - 1;
}

auto Interface::UploadMeshData(Vertex *vertices, size_t vCount, Index *indices,
                               size_t iCount) -> uint64_t {
  std::vector<Vertex> vertBuffer = {};

  // 9 floats = 1 vert
  for (int x = 0; x < vCount; x++) {
    Vertex v{};
    std::memcpy(v.Position, vertices[x].Position, 4 * sizeof(float));
    std::memcpy(v.Normal, vertices[x].Normal, 3 * sizeof(float));
    std::memcpy(v.UV, vertices[x].UV, 2 * sizeof(float));
    vertBuffer.push_back(v);
  }

  std::vector<std::uint32_t> indBuffer = {};

  for (int x = 0; x < iCount; x++) {
    indBuffer.push_back(indices[x]);
  }

  size_t vertSize = vertBuffer.size() * sizeof(Vertex);
  auto vertUpload = _device->CreateUploadBuffer(vertSize);
  size_t indSize = indBuffer.size() * sizeof(uint32_t);
  auto indUpload = _device->CreateUploadBuffer(indSize);

  _uploadBuffers.push_back(vertUpload);
  _uploadBuffers.push_back(indUpload);

  auto vao = std::make_shared<VertexArrayObject>(
      _resourceCounter, _device->CreateVertexBuffer(vertBuffer),
      _device->CreateIndexBuffer(indBuffer));
  _uploadCommandList->UpdateSubresources(vao->VertexBuffer, vertUpload,
                                         vertices,
                                         vertBuffer.size() * sizeof(Vertex), 1);
  _uploadCommandList->Transition(static_pointer_cast<Buffer>(vao->VertexBuffer),
                                 ResourceState::COPY_DEST,
                                 ResourceState::VERTEX_CONST_BUFFER);

  _uploadCommandList->UpdateSubresources(vao->IndexBuffer, indUpload, indices,
                                         iCount * sizeof(uint32_t), 1);
  _uploadCommandList->Transition(static_pointer_cast<Buffer>(vao->IndexBuffer),
                                 ResourceState::COPY_DEST,
                                 ResourceState::VERTEX_CONST_BUFFER);

  _meshes.emplace_back(vao);

  return _meshes.size() - 1;
}

auto Interface::UploadTextureData(uint8_t *data, uint16_t width,
                                  uint16_t height, uint8_t channels)
    -> uint64_t {
  auto texBuff =
      _device->CreateTextureBuffer(width, height, channels, 0, Renderer::RGBA);
  auto uploadBuffer = _device->CreateUploadBuffer(width * height * channels);

  _uploadCommandList->Transition(static_pointer_cast<TextureBuffer>(texBuff),
                                 ResourceState::COMMON,
                                 ResourceState::COPY_DEST);

  _uploadCommandList->UpdateSubresources(
      texBuff, uploadBuffer, data, width * channels, width * channels * height);

  _device->CreateShaderResourceView(texBuff,
                                    _srvHeap->CpuHandleFor(_srvCounter));
  texBuff->GPUHandle = _srvHeap->GpuHandleFor(_srvCounter);
  _uploadCommandList->Transition(static_pointer_cast<TextureBuffer>(texBuff),
                                 ResourceState::COPY_DEST,
                                 ResourceState::PIXEL_SHADER);

  _srvCounter++;
  //_device->CreateMipMaps(texBuff, _computeCommandList, width, height, width /
  // 256);

  _textures.push_back(texBuff);
  _uploadBuffers.push_back(uploadBuffer);

  return _textures.size() - 1;
}

auto Interface::UploadShaderData(GraphicsShader shader) -> uint64_t {
      auto compiledShader = _device->CompileShader(shader.Code);
      compiledShader->ShaderIndex = _shaders.size();
    

  GraphicsRootSignatureDescription desc = {};
  desc.Parameters = {};
  // MVP Matrix

  auto sizes = {sizeof(MVPCBuffer) / 4, sizeof(MaterialBuffer) / 4,
                sizeof(LightBuffer) / 4};

  // Add default shader-agnostic parameters.
  // TODO: Parse shader json and add additional params

  GraphicsRootSignatureParameter param = {};

  // --- MVP MATRIX ---
  param.Index = 0;
  param.Size = sizeof(MVPCBuffer) / 4;
  param.ShaderRegister = 0;
  param.RegisterSpace = 0;
  param.Visibility = GraphicsShaderVisibility::VERTEX;
  param.Type = GraphicsRootSignatureParameterType::CONSTANT;
  param.RangeType = GraphicsDescriptorRangeType::CBV;

  desc.Parameters.push_back(param);

  // --- USER DEFINED CBV ---
  param.Index = 1;
  param.Size = 1;
  param.ShaderRegister = 1;
  param.RegisterSpace = 0;
  param.Visibility = GraphicsShaderVisibility::ALL;
  param.Type = GraphicsRootSignatureParameterType::TABLE;
  param.RangeType = GraphicsDescriptorRangeType::CBV;

  desc.Parameters.push_back(param);

  // SHADER RESOURCE VIEW FOR CBV
  param.Index = 2;
  param.Size = 1;
  param.ShaderRegister = 0;
  param.RegisterSpace = 0;
  param.Visibility = GraphicsShaderVisibility::PIXEL;
  param.Type = GraphicsRootSignatureParameterType::TABLE;
  param.RangeType = GraphicsDescriptorRangeType::SRV;

  desc.Parameters.push_back(param);

  // TEXTURE 0 s0
  param.Index = 3;
  param.Size = 1;
  param.ShaderRegister = 1;
  param.RegisterSpace = 0;
  param.Visibility = GraphicsShaderVisibility::PIXEL;
  param.Type = GraphicsRootSignatureParameterType::TABLE;
  param.RangeType = GraphicsDescriptorRangeType::SRV;

  desc.Parameters.push_back(param);

  auto signature = _device->CreateGraphicsRootSignature(desc);

  std::vector<GraphicsPipelineInputElement> inputElements = {
      {"POSITION", 0, GraphicsPipelineFormat::RGBA32_FLOAT, 0, 0,
       GraphicsPipelineClassification::VERTEX, 0},
      {"NORMAL", 0, GraphicsPipelineFormat::RGB32_FLOAT, 0, 16,
       GraphicsPipelineClassification::VERTEX, 0},
      {"TEXCOORD", 0, GraphicsPipelineFormat::RG32_FLOAT, 0, 28,
       GraphicsPipelineClassification::VERTEX, 0}};

  auto pipeline =
      _device->CreatePipelineState(signature, inputElements, compiledShader);
  compiledShader->Pipeline = pipeline;

  _shaders.push_back(compiledShader);

  return _shaders.size() - 1;
}

auto Interface::DrawMesh(uint64_t id, uint64_t materialId, MeshDrawInfo info,
                         glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    -> void {

  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(transform, position);

  _meshesToDraw.push_back({id, materialId, transform});
}

auto Interface::SetMeshProperties() -> void {}

auto Interface::SetCamera(glm::vec3 position, glm::vec3 rotation) -> void {
  camPos = position;
  camRot = rotation;
}

auto Interface::MeshDataFor(const char *uuid) -> std::vector<uint64_t> {
  return {};
}

auto Interface::CreatePipeline() -> void {
  _resourceCounter = 0;
  _mainQueue = _device->CreateCommandQueue(DIRECT);

  _computeQueue = _device->CreateCommandQueue(COMPUTE);
  _computeAllocator = _device->CreateCommandAllocator(COMPUTE);
  _computeCommandList =
      _device->CreateCommandList(COMPUTE, _computeAllocator, "Compute Queue");

  _swapChain = _device->CreateSwapChain(_mainQueue, FRAME_COUNT,
                                        _windowDimension.x, _windowDimension.y);
  _frameIndex = _swapChain->CurrentBackBufferIndex();

  _rtvHeap = _device->CreateRenderTargetHeap(FRAME_COUNT);
  _srvHeap = _device->CreateShaderResourceHeap(128);
  _uavHeap = _device->CreateUnorderedAccessHeap(100);
  _cbvHeap = _device->CreateConstantBufferHeap(128);
  _dsvHeap = _device->CreateDepthStencilHeap(1);
  _samplerHeap = _device->CreateSamplerHeap(128);
  _meshesToDraw = {};

  _depthBuffer = _device->CreateDepthBuffer(_windowDimension);

  _device->CreateDepthStencilView(_dsvHeap, _depthBuffer);

  for (uint32_t x = 0; x < FRAME_COUNT; x++) {
    auto rt = _swapChain->GetBuffer(x);
    auto rtHandle = _rtvHeap->CpuHandleFor(x);
    _device->CreateRenderTarget(rtHandle, rt);
    auto alloc = _device->CreateCommandAllocator(DIRECT);
    _frames[x] = _device->CreateFrame(alloc, rt);
  }

  _scissorRect = {0u, 0u, static_cast<uint32_t>(_windowDimension.x),
                  static_cast<uint32_t>(_windowDimension.y)};
  _viewport = {0.0f,
               0.0f,
               static_cast<float>(_windowDimension.x),
               static_cast<float>(_windowDimension.y),
               0.0f,
               1.0f};

  _fence = _device->CreateFence(_frames[0]->FenceValue());
  _uploadFence = _device->CreateFence(_uploadFenceValue);

  _uploadAllocator = _device->CreateCommandAllocator(DIRECT);
  _commandList = _device->CreateCommandList(
      DIRECT, _frames[_frameIndex]->Allocator(), "CommandList");
  _uploadCommandList =
      _device->CreateCommandList(DIRECT, _uploadAllocator, "UploadCommandList");

  _commandList->Close();
  _uploadCommandList->Close();
  auto commandLists = {_commandList};
  _mainQueue->ExecuteCommandLists(commandLists);

  _frames[_frameIndex]->IncrementFenceValue();

  // Create an event handle to use for frame synchronization.
  _fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  if (_fenceEvent == nullptr) {
    HRESULT_FROM_WIN32(GetLastError());
  }

  _uploadFenceValue = 0;
  _uploadFence = _device->CreateFence(_uploadFenceValue);
#if _WIN32
  _uploadFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  if (_uploadFenceEvent == nullptr) {
    HRESULT_FROM_WIN32(GetLastError());
  }
#endif

  _uploadAllocator->Reset();
  _uploadCommandList->Reset(_uploadAllocator, nullptr);

  _computeFenceValue = 0;
  _computeFence = _device->CreateFence(_computeFenceValue);
#if _WIN32
  _computeFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  if (_computeFenceEvent == nullptr) {
    HRESULT_FROM_WIN32(GetLastError());
  }
#endif

  _computeCommandList->Close();

  _computeAllocator->Reset();
  _computeCommandList->Reset(_computeAllocator, nullptr);

  for (uint32_t x = 0; x < FRAME_COUNT; x++) {
    _cbAllocator[x] = _device->CreateCommandAllocator(DIRECT);
    _lightDataBuffer[x] =
        _device->CreateUploadBuffer((sizeof(LightBuffer) + 255) & ~255);
    _cbAllocator[x]->Reset();

    _device->CreateConstantBufferView(_srvHeap, _lightDataBuffer[x],
                                      _srvHeap->CpuHandleFor(_srvCounter++));
  }

  _cbCommandList = _device->CreateCommandList(DIRECT, _cbAllocator[_frameIndex],
                                              "Constant Buffer CommandList");
  _cbCommandList->Close();

  WaitForGPU(_mainQueue);
  WaitForGPU(_computeQueue);

#if _WIN32
  auto device = static_pointer_cast<D3D12Device>(_device)->Native();
  auto heap = static_pointer_cast<D3D12Heap>(_srvHeap)->Native();
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(reinterpret_cast<SDL_Window *>(_window), &wmInfo);
  HWND hwnd = wmInfo.info.win.window;
  ImGui_ImplWin32_Init(hwnd);
  auto cpuHandle =
      static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(
          _srvHeap->CpuHandleFor(_srvCounter));
  auto gpuHandle =
      static_pointer_cast<DescriptorHandleT<D3D12_GPU_DESCRIPTOR_HANDLE>>(
          _srvHeap->GpuHandleFor(_srvCounter));
  ImGui_ImplDX12_Init(device, FRAME_COUNT, DXGI_FORMAT_R8G8B8A8_UNORM, heap,
                      cpuHandle->Handle(), gpuHandle->Handle());
  _srvCounter++;
#endif
}

auto Interface::CreateAssets() -> void {}

auto Interface::CreateUI(void *context) -> void {}

auto Interface::FillCommandList() -> void {}

auto Interface::MoveToNextFrame() -> void {
  const uint64_t currentFenceValue = _frames[_frameIndex]->FenceValue();
  _mainQueue->Signal(_fence, currentFenceValue);

  // Update the frame index.
  _frameIndex = _swapChain->CurrentBackBufferIndex();

  auto target = _frames[_frameIndex]->FenceValue();
  // If the next frame is not ready to be rendered yet, wait until it is ready.
  _fence->Wait(_frames[_frameIndex]->FenceValue(), _fenceEvent);

  // Set the fence value for the next frame.
  _frames[_frameIndex]->IncrementFenceValue();
}

auto Interface::GenerateMipMaps() -> void {}

auto Interface::WaitForGPU(std::shared_ptr<CommandQueue> queue) -> void {
  queue->Signal(_fence, _frames[_frameIndex]->FenceValue());

  // Wait until the fence has been processed.
  _fence->SetOnCompletion(_frames[_frameIndex]->FenceValue(), _fenceEvent);
  _fence->Wait(_frames[_frameIndex]->FenceValue(), _fenceEvent);
  // Increment the fence value for the current frame.
  auto _ = _frames[_frameIndex]->IncrementFenceValue();
}

} // namespace Renderer
