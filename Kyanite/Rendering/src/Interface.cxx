#include "Interface.hxx"
#include "DescriptorHandle.hxx"
#include "Mesh.hxx"
#include "Rect.hxx"
#include "Renderer.hxx"
#include "Vertex.hxx"
#include "Viewport.hxx"

#include "d3d12/D3D12GraphicsCommandList.hxx"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/common.hpp"
#include "glm/mat4x4.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "glm/vec4.hpp"
#include <SDL_syswm.h>
#include <algorithm>
#include <d3d12.h>
#include <debugapi.h>
#include <dxgiformat.h>
#include <glm/ext/quaternion_transform.hpp>
#include <thread>

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
#define WIN32_LEAN_AND_MEAN
#include "d3d12/D3D12Device.hxx"
#include <Windows.h>

#endif
#include <d3dx12.h>

#include <clocale>

#include "d3d12/D3D12Heap.hxx"
#include <iostream>

float xPos = 1;
float yPos = 50;
float zPos = -5;
glm::vec4 sunColor = {1, 0.7f, 0.7f, 1};
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

Interface::Interface(uint32_t width, uint32_t height, void *window, RenderBackendAPI type) {
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
  //ImGui::SetCurrentContext((reinterpret_cast<ImGuiContext*>(ctx)));

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
  _mouseOverAllocator->Reset();
  _commandList->Reset(_frames[_frameIndex]->Allocator(), nullptr);
  _imguiAllocator->Reset();
  _imguiCommandList->Reset(_imguiAllocator, nullptr);
  _mouseOverCommandList->Reset(_mouseOverAllocator, nullptr);
  _cbAllocator[_frameIndex]->Reset();
  _cbCommandList->Reset(_cbAllocator[_frameIndex], nullptr);
}

auto Interface::MidFrame() -> void {
  _commandList->SetDescriptorHeaps({_srvHeap});
  _imguiCommandList->SetDescriptorHeaps({ _srvHeap });
  _mouseOverCommandList->SetDescriptorHeaps({_srvHeap});

  auto sun = SunLight{sunColor, {xPos, yPos, zPos, 1}, true};

  light.Ambient = {ambientColor, ambientIntensity};

  light.Sun = sun;
  glm::vec3 cameraPos = {camPos.x, camPos.y, camPos.z - 20};
  glm::vec3 cameraFront = {0.0f, 0.0f, 1.0f};
  glm::vec3 cameraLookAt = camPos;
  glm::vec3 cameraUp = {0.0f, 1.0f, 0.0f};

  _viewMatrix = glm::lookAtLH(cameraPos, cameraLookAt, cameraUp);
  //_viewMatrix = glm::inverse(_viewMatrix);
  _projectionMatrix =
      glm::perspectiveFovLH(glm::radians(55.0f), (float)_windowDimension.x,
                            (float)_windowDimension.y, 0.01f, 1000.0f);

  // Record commands.
  glm::vec4 clearColor = {255, 255, 255, 255};

  // --- Render pass for mouse over ---
  if(_colorOnlyShader == nullptr) {
      auto colorShader = std::find_if(_shaders.begin(), _shaders.end(), [](auto& shader) { 
        return shader->Name == "_COLOR_ONLY";
      });

      _colorOnlyShader = colorShader != _shaders.end() ? *colorShader : nullptr;
  }

  if(_colorOnlyShader != nullptr) {
    
    _mouseOverCommandList->Transition(_mouseOverBuffer[_frameIndex], ResourceState::COPY_SOURCE,
                             ResourceState::RENDER_TARGET);
    auto rtvHandle = _rtvHeap->CpuHandleFor(FRAME_COUNT + _frameIndex);
    auto dsvHandle = _dsvHeap->CpuHandleFor(1);
    _mouseOverCommandList->SetRenderTarget(rtvHandle, dsvHandle);
    _mouseOverCommandList->ClearRenderTarget(rtvHandle, clearColor);
    _mouseOverCommandList->ClearDepthTarget(dsvHandle);
    _mouseOverCommandList->SetTopology(GraphicsPipelineStateTopology::POLYGON);
    _mouseOverCommandList->SetViewport(_viewport);

    Rect r = {};
    r.Left = _cursorPosition[0];
    r.Top = _cursorPosition[1];
    r.Right = _cursorPosition[0] + 1;
    r.Bottom = _cursorPosition[1] + 1;
    _mouseOverCommandList->SetScissorRect(_scissorRect);
    //_mouseOverCommandList->SetScissorRect(_scissorRect);

    for (auto &mesh : _meshesToDraw) {
      std::array<uint32_t, 4> entityColorArr = {
        static_cast<uint8_t>((mesh.EntityId)  & 0xFF), static_cast<uint8_t>((mesh.EntityId >> 8) & 0xFF), static_cast<uint8_t>((mesh.EntityId >> 16) & 0xFF), static_cast<uint8_t>((mesh.EntityId >> 24) & 0xFF)
      };
      // Update the projection matrix.
      float aspectRatio =
          _windowDimension.x / static_cast<float>(_windowDimension.y);
      // Update the MVP matrix

      glm::mat4 modelView = mesh.Transform;
      modelView = glm::scale(modelView, {1, 1, 1});
      modelView = glm::translate(modelView, {0, 0, 0});
      modelView = glm::rotate(modelView, glm::radians(0.0f),
                              glm::vec3{1.0f, 0.0f, 0.0f});
      //

      auto mvp = MVPCBuffer{modelView, _projectionMatrix * _viewMatrix};

      auto material = _materials[mesh.MaterialId];
      auto shader = _colorOnlyShader;

      _mouseOverCommandList->SetGraphicsRootSignature(
          shader->Pipeline->RootSignature());
      _mouseOverCommandList->SetPipelineState(shader->Pipeline);
      _mouseOverCommandList->SetGraphicsRootConstants(
          0, sizeof(mvp) / 4, &mvp,
          0); // b0     -> The Model View Projection for this Mesh/Material
       _mouseOverCommandList->SetGraphicsRootConstants(1, 4, &entityColorArr, 0);



      auto vao = _meshes[mesh.MeshId];
      _mouseOverCommandList->SetVertexBuffer(vao->VertexBuffer);
      _mouseOverCommandList->SetIndexBuffer(vao->IndexBuffer);
      _mouseOverCommandList->DrawInstanced(vao->IndexBuffer->Size(), 1, 0, 0, 0);
    }

    _mouseOverCommandList->Transition(_mouseOverBuffer[_frameIndex],
                             ResourceState::RENDER_TARGET, ResourceState::COPY_SOURCE);
    // Copy Resulting Render Texture to a readbackbuffer in order for CPU to detect the mouse over.
    _mouseOverCommandList->Copy(0, 0, _windowDimension.x, _windowDimension.y, _mouseOverBuffer[_frameIndex], _mouseOverReadbackBuffer);
  }

  // --- Display Render Pass ---
  {
    clearColor = {0, 0.2, 0.2, 1};
      _commandList->SetGraphicsRootConstantBuffer(
      _lightDataBuffer[_frameIndex], &light, sizeof(light),
      _lightDataGPUAddress[_frameIndex]);

      _commandList->Transition(_frames[_frameIndex]->RenderTarget(),
                               ResourceState::PRESENT,
                               ResourceState::RENDER_TARGET);

    auto dsvHandle = _dsvHeap->CpuHandleFor(0);
    auto rtvHandle = _rtvHeap->CpuHandleFor(_frameIndex);
    _commandList->SetRenderTarget(rtvHandle, dsvHandle);
    _commandList->SetTopology(GraphicsPipelineStateTopology::POLYGON);
    _commandList->SetViewport(_viewport);
    _commandList->ClearRenderTarget(rtvHandle, clearColor);
    _commandList->ClearDepthTarget(dsvHandle);
    _commandList->SetScissorRect(_scissorRect);

    for (auto &mesh : _meshesToDraw) {
      // Update the projection matrix.
      float aspectRatio =
          _windowDimension.x / static_cast<float>(_windowDimension.y);
      // Update the MVP matrix

      glm::mat4 modelView = mesh.Transform;
      modelView = glm::scale(modelView, {1, 1, 1});
      modelView = glm::translate(modelView, {0, 0, 0});
      modelView = glm::rotate(modelView, glm::radians(0.0f),
                              glm::vec3{1.0f, 0.0f, 0.0f});
      //

      auto mvp = MVPCBuffer{modelView, _projectionMatrix * _viewMatrix};

      auto material = _materials[mesh.MaterialId];
      auto shader = material->Shader;

      _commandList->SetGraphicsRootSignature(shader->Pipeline->RootSignature());
      _commandList->SetPipelineState(shader->Pipeline);
      _commandList->SetGraphicsRootConstants(
          0, sizeof(mvp) / 4, &mvp,
          0); // b0     -> The Model View Projection for this Mesh/Material

      if (shader->IsLit) {
        _commandList->SetGraphicsRootDescriptorTable(
            1,
            _srvHeap->GpuHandleFor(
                2)); // b1     -> The Constant Buffer
                               // for this Mesh/Material Light data
      }
      // FIXME: The Texture offsets might start earlier if there is no light CBV
      // andor no User-defined CBV
      // FIXME: Currently, the user-defined CBV is skipped. Need to implement
      // that later on


      for (auto [it, end, x] = std::tuple{material->Textures.cbegin(),
                                          material->Textures.cend(), 0};
           it != end; it++, x++) {
        auto texture = *it;
        auto slotBinding = std::find_if(
            material->Shader->Constants.cbegin(), material->Shader->Constants.cend(),
            [texture](auto &item) { return item.Name == texture.first; });

        if (slotBinding != material->Shader->Constants.end()) {
          auto index = 2 + (shader->ConstantBufferLayout.empty() ? 0 : 1)  + slotBinding->Index + (shader->IsLit ? 1 : 0);
          _commandList->SetGraphicsRootDescriptorTable(index,it->second->GPUHandle);
        }
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
  }

  // ImGui Render Pass
  {
      _imguiCommandList->Transition(
          _frames[_frameIndex]->RenderTarget(),
          ResourceState::PRESENT,
          ResourceState::RENDER_TARGET);

      auto dsvHandle = _dsvHeap->CpuHandleFor(0);
      auto rtvHandle = _rtvHeap->CpuHandleFor(_frameIndex);
      _imguiCommandList->SetRenderTarget(rtvHandle, dsvHandle);
      _imguiCommandList->SetTopology(GraphicsPipelineStateTopology::POLYGON);
      _imguiCommandList->SetViewport(_viewport);
      _imguiCommandList->ClearRenderTarget(rtvHandle, clearColor);
      _imguiCommandList->ClearDepthTarget(dsvHandle);
      _imguiCommandList->SetScissorRect(_scissorRect);
      ImGui::Render();

#if _WIN32
      auto cmdList =
          static_pointer_cast<D3D12GraphicsCommandList>(_imguiCommandList)->Native();
      ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
#endif

      _imguiCommandList->Transition(
          _frames[_frameIndex]->RenderTarget(),
          ResourceState::RENDER_TARGET,
          ResourceState::PRESENT
      );
  }

  // Indicate that the back buffer will now be used to present.
  bool copyFrameSet = false;
  auto copyBackBuffFrameIndex = _frameIndex;
  if (copyBackBuffFrameIndex >= FRAME_COUNT - 1) {
      copyFrameSet = true;
      copyBackBuffFrameIndex = 0;
  }
  if (!copyFrameSet && copyBackBuffFrameIndex <= 0) {
      copyBackBuffFrameIndex = FRAME_COUNT - 1;
  }

  _nextFrameIndex = copyBackBuffFrameIndex;
  
  _commandList->Transition(_frames[_frameIndex]->RenderTarget(),
      ResourceState::RENDER_TARGET,
      ResourceState::COPY_SOURCE);
  
  _commandList->Copy(0, 0, _windowDimension.x, _windowDimension.y, _frames[_frameIndex]->RenderTarget(), _textures[_nextFrameIndex]);

  _commandList->Transition(_frames[_frameIndex]->RenderTarget(),
      ResourceState::COPY_SOURCE,
      ResourceState::PRESENT);

  _mouseOverCommandList->Close();
  _commandList->Close();
  _imguiCommandList->Close();
  _cbCommandList->Close();

  //_mainQueue->ExecuteCommandLists({_mouseOverCommandList, _commandList, _imguiCommandList });
  _mainQueue->ExecuteCommandLists({_mouseOverCommandList, _commandList, _imguiCommandList });

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

auto Interface::Resize(uint32_t width, uint32_t height) -> void {
    // Flush the GPU first -> It has to be idle
    FlushGPU();
    _windowDimension = {width, height};

    _depthBuffer = _device->CreateDepthBuffer(_windowDimension, "Depth Buffer");
    _device->CreateDepthStencilView(_dsvHeap->CpuHandleFor(0), _depthBuffer);
    for (int x = 0; x < FRAME_COUNT; x++) {
        _frames[x]->ResetRenderTarget();
    }
    _swapChain->Resize(FRAME_COUNT, width, height);
    _scissorRect = { 0u, 0u, static_cast<uint32_t>(_windowDimension.x),
                static_cast<uint32_t>(_windowDimension.y) };

    for (uint32_t x = 0; x < FRAME_COUNT; x++) {
        auto rt = _swapChain->GetBuffer(x);
        auto rtHandle = _rtvHeap->CpuHandleFor(x);
        _device->CreateRenderTargetView(rtHandle, rt);
        _frames[x]->SetRenderTarget(rt);
    }

    _frameIndex = _swapChain->CurrentBackBufferIndex();

    _textures[0] = _device->CreateTextureBuffer(_windowDimension.x, _windowDimension.y, 4, 0, Renderer::RGBA, "Current Back Buffer");
    _textures[1] = _device->CreateTextureBuffer(_windowDimension.x, _windowDimension.y, 4, 0, Renderer::RGBA, "Current Back Buffer");
    
    _device->CreateShaderResourceView(_textures[0], _srvHeap->CpuHandleFor(0));
    _device->CreateShaderResourceView(_textures[1], _srvHeap->CpuHandleFor(1));
}

auto Interface::CreateMaterial(std::string name, uint64_t shaderId) -> uint64_t {
  auto mat = std::make_shared<Material>();
  mat->Name = name;
  mat->Shader = _shaders[shaderId];

  if (!mat->Shader->ConstantBufferLayout.empty()) {

    std::sort(mat->Shader->ConstantBufferLayout.begin(),
              mat->Shader->ConstantBufferLayout.end(),
              [](auto &lhs, auto &rhs) { return lhs.Index < rhs.Index; });

    size_t buffSize = 0;

    // Props are 16 byte alligned -> Vector 4 = 4x4 Bytes = 16. Buffer must be
    // multiple of 255 bytes as well
    buffSize = ((mat->Shader->ConstantBufferLayout.size() * 16) + 255) & ~255;
    _materialCBVs.insert(
        {_materials.size(), _device->CreateUploadBuffer(buffSize, name.c_str())});

    _device->CreateConstantBufferView(_srvHeap,
                                      _materialCBVs.at(_materials.size()),
                                      _srvHeap->CpuHandleFor(_srvCounter++));

    std::vector<uint8_t> materialBuffer(buffSize);
    _materialBuffers.insert({_materials.size(), materialBuffer});
  }
  _materials.push_back(mat);
  return _materials.size() - 1;
}

auto Interface::SetMaterialTexture(uint64_t material, std::string name,
                                   uint64_t texture) -> void {
  _materials[material]->Textures[name] = _textures[texture];
}

auto Interface::SetMaterialFloat(uint64_t material, std::string name,
                                 float value) -> void {}
auto Interface::SetMaterialVector2(uint64_t material, std::string name,
                                   float *value) -> void {}
auto Interface::SetMaterialVector3(uint64_t material, std::string name,
                                   float *value) -> void {}
auto Interface::SetMaterialVector4(uint64_t material, std::string name,
                                   float *value) -> void {}
auto Interface::SetMaterialInt(uint64_t material, std::string name, int value)
    -> void {}
auto Interface::SetMaterialBool(uint64_t material, std::string name, bool value)
    -> void {}

auto Interface::UploadMeshData(Vertex *vertices, size_t vCount, Index *indices,
                               size_t iCount) -> uint64_t {
  std::vector<Vertex> vertBuffer = {};

  // 9 floats = 1 vert
  for (int x = 0; x < vCount; x++) {
    Vertex v{};
    std::memcpy(v.Position, vertices[x].Position, 4 * sizeof(float));
    std::memcpy(v.Normal, vertices[x].Normal, 3 * sizeof(float));
    std::memcpy(v.UV, vertices[x].UV, 2 * sizeof(float));
    std::memcpy(v.Color, vertices[x].Color, 4 * sizeof(float));
    vertBuffer.push_back(v);
  }

  std::vector<std::uint32_t> indBuffer = {};

  for (int x = 0; x < iCount; x++) {
    indBuffer.push_back(indices[x]);
  }

  size_t vertSize = vertBuffer.size() * sizeof(Vertex);
  auto vertUpload = _device->CreateUploadBuffer(vertSize, "UploadBuffer");
  size_t indSize = indBuffer.size() * sizeof(uint32_t);
  auto indUpload = _device->CreateUploadBuffer(indSize, "UploadBuffer");

  _uploadBuffers.push_back(vertUpload);
  _uploadBuffers.push_back(indUpload);

  auto vao = std::make_shared<VertexArrayObject>(
      _resourceCounter, _device->CreateVertexBuffer(vertBuffer, "VertexBuffer"),
      _device->CreateIndexBuffer(indBuffer, "IndexBuffer"));
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
      _device->CreateTextureBuffer(width, height, channels, 0, Renderer::RGBA, "TextureBuffer");
  auto uploadBuffer = _device->CreateUploadBuffer(width * height * channels, "TextureUploadBuffer");

  _uploadCommandList->Transition(static_pointer_cast<TextureBuffer>(texBuff),
                                 ResourceState::COMMON,
                                 ResourceState::COPY_DEST);

  _uploadCommandList->UpdateSubresources(
      texBuff, uploadBuffer, data, width * channels, width * channels * height);

  auto index = _srvCounter++;
  _device->CreateShaderResourceView(texBuff,
                                    _srvHeap->CpuHandleFor(index));
  texBuff->GPUHandle = _srvHeap->GpuHandleFor(index);
  _uploadCommandList->Transition(static_pointer_cast<TextureBuffer>(texBuff),
                                 ResourceState::COPY_DEST,
                                 ResourceState::PIXEL_SHADER);

  //_device->CreateMipMaps(texBuff, _computeCommandList, width, height, width /
  // 256);

  _textures.push_back(texBuff);
  _uploadBuffers.push_back(uploadBuffer);

  return _textures.size() - 1;
}

auto Interface::UploadShaderData(GraphicsShader shader) -> uint64_t {
  auto compiledShader = _device->CompileShader(shader.Code);
  compiledShader->ShaderIndex = _shaders.size();
  compiledShader->Name = shader.Name;
  compiledShader->IsLit = shader.IsLit;
  compiledShader->Format = shader.Format;

  GraphicsRootSignatureDescription desc = {};
  desc.Parameters = {};
  // MVP Matrix

  // Add default shader-agnostic parameters.
  // TODO: Parse shader json and add additional params

  GraphicsRootSignatureParameter param = {};

  uint8_t rootConstantIndex = 0; 
  uint8_t shaderRegisterB = 0;
  uint8_t shaderRegisterS = 0;
  // --- MVP MATRIX ---
  param.Index = rootConstantIndex++;
  param.Size = sizeof(MVPCBuffer) / 4;
  param.ShaderRegister = shaderRegisterB++;
  param.RegisterSpace = 0;
  param.Visibility = GraphicsShaderVisibility::VERTEX;
  param.Type = GraphicsRootSignatureParameterType::CONSTANT;
  param.RangeType = GraphicsDescriptorRangeType::CBV;

  desc.Parameters.push_back(param);

  if (compiledShader->IsLit) {
    // --- LIGHT DATA CBV ---
    param.Index = rootConstantIndex++;
    param.Size = 1;
    param.ShaderRegister = shaderRegisterB++;
    param.RegisterSpace = 0;
    param.Visibility = GraphicsShaderVisibility::ALL;
    param.Type = GraphicsRootSignatureParameterType::TABLE;
    param.RangeType = GraphicsDescriptorRangeType::CBV;

    desc.Parameters.push_back(param);

    // --- SHADER RESOURCE VIEW FOR LIGHT DATA CBV ---
    param.Index = rootConstantIndex++;
    param.Size = 1;
    param.ShaderRegister = shaderRegisterS++;
    param.RegisterSpace = 0;
    param.Visibility = GraphicsShaderVisibility::PIXEL;
    param.Type = GraphicsRootSignatureParameterType::TABLE;
    param.RangeType = GraphicsDescriptorRangeType::SRV;

    desc.Parameters.push_back(param);
  }

  for (auto &item : shader.Constants) {
    // SKip Textures, those belond to SRV, not Constants
    if(item.Type == GraphicsShaderSlotType::TEXTURE) {
      continue;
    }
    size_t size = 0;
    switch(item.Type) {
      case GraphicsShaderSlotType::INT:
      case GraphicsShaderSlotType::BOOL:
      case GraphicsShaderSlotType::FLOAT:
        size = 1;
        break;
      case GraphicsShaderSlotType::VECTOR2:
        size = 2;
        break;
      case GraphicsShaderSlotType::VECTOR3:
        size = 3;
        break;
      case GraphicsShaderSlotType::VECTOR4:
        size = 4;
        break;
      default:
        throw std::runtime_error("Textures are not meant to be bound to the root signature via constants");
    }
    param.Index = rootConstantIndex++;
    param.Size = size;
    param.ShaderRegister = shaderRegisterB++;
    param.RegisterSpace = 0;
    param.Visibility = GraphicsShaderVisibility::ALL;
    param.Type = GraphicsRootSignatureParameterType::CONSTANT;
    param.RangeType = GraphicsDescriptorRangeType::CBV;

    desc.Parameters.push_back(param);
  }

  if (!shader.ConstantBufferLayout.empty()) {

    // --- USER-DEFINED CBV ---
    param.Index = rootConstantIndex++;
    param.Size = 1;
    param.ShaderRegister = shaderRegisterB++;
    param.RegisterSpace = 0;
    param.Visibility = GraphicsShaderVisibility::ALL;
    param.Type = GraphicsRootSignatureParameterType::TABLE;
    param.RangeType = GraphicsDescriptorRangeType::CBV;

    desc.Parameters.push_back(param);

    // --- SHADER RESOURCE VIEW FOR USER-DEFINED CBV ---
    param.Index = rootConstantIndex++;
    param.Size = 1;
    param.ShaderRegister = shaderRegisterS++;
    param.RegisterSpace = 0;
    param.Visibility = GraphicsShaderVisibility::PIXEL;
    param.Type = GraphicsRootSignatureParameterType::TABLE;
    param.RangeType = GraphicsDescriptorRangeType::SRV;

    desc.Parameters.push_back(param);
  }

  // --- TEXTURE 0 s0 ---
  for (auto& item: shader.Constants) {
    if (item.Type == GraphicsShaderSlotType::TEXTURE) {
      param.Index = rootConstantIndex++;
      param.Size = 1;
      param.ShaderRegister = shaderRegisterS++;
      param.RegisterSpace = 0;
      param.Visibility = GraphicsShaderVisibility::PIXEL;
      param.Type = GraphicsRootSignatureParameterType::TABLE;
      param.RangeType = GraphicsDescriptorRangeType::SRV;

      desc.Parameters.push_back(param);
    }
  }

  auto signature = _device->CreateGraphicsRootSignature(desc);

  std::vector<GraphicsPipelineInputElement> inputElements = {
      {"POSITION", 0, GraphicsPipelineFormat::RGBA32_FLOAT, 0, 0, GraphicsPipelineClassification::VERTEX, 0},
      {"NORMAL", 0, GraphicsPipelineFormat::RGB32_FLOAT, 0, 16,GraphicsPipelineClassification::VERTEX, 0},
      {"TEXCOORD", 0, GraphicsPipelineFormat::RG32_FLOAT, 0, 28,GraphicsPipelineClassification::VERTEX, 0},
      {"COLOR", 0, GraphicsPipelineFormat::RGBA32_FLOAT, 0, 36, GraphicsPipelineClassification::VERTEX, 0},
      {"SV_InstanceID", 0, GraphicsPipelineFormat::R32_UINT, 0, 52, GraphicsPipelineClassification::VERTEX, 0}
      };

  auto pipeline =
      _device->CreatePipelineState(signature, inputElements, compiledShader, compiledShader->Format);
  compiledShader->Pipeline = pipeline;

  compiledShader->Constants = shader.Constants;
  compiledShader->ConstantBufferLayout = shader.ConstantBufferLayout;
  _shaders.push_back(compiledShader);

  return _shaders.size() - 1;
}

auto Interface::DrawMesh(uint64_t entityId, uint64_t meshId,
                         uint64_t materialId, MeshDrawInfo info,
                         glm::vec3 position, glm::quat rotation,
                         glm::vec3 scale) -> void {

  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(transform, position);
  transform = glm::scale(transform, scale);
  auto axis = glm::eulerAngles(rotation);
  glm::mat4 transformX = glm::eulerAngleX(axis.x);
  glm::mat4 transformY = glm::eulerAngleY(axis.y);
  glm::mat4 transformZ = glm::eulerAngleZ(axis.z);
  transform = transform * transformY * transformX * transformZ;

  _meshesToDraw.push_back({entityId, meshId, materialId, transform});
}

auto Interface::DrawLine(glm::vec3 from, glm::vec3 to, glm::vec4 color) -> void {

}

auto Interface::SetCursorPosition(std::array<uint32_t, 2> position) -> void {
  _cursorPosition = position;
}

auto Interface::ReadMouseOverData(uint32_t x, uint32_t y) -> uint32_t {
  auto index = (
    _cursorPosition[0] // THE X Location of our target pixel.
    + (int)(_windowDimension.x * _cursorPosition[1]) + (_cursorPosition[1] * (_mouseOverPadding))
    );
  auto mouseOverData = _mouseOverReadbackBuffer->ReadBytes();
  return (mouseOverData[index]);
}

auto Interface::GetOutputTexture() -> uint64_t {
    return _srvHeap->GpuHandleFor(_frameIndex)->Address();
}

auto Interface::SetMainViewport(uint32_t startX, uint32_t startY, uint32_t width, uint32_t height) -> void{
    _viewport = { (float)startX, (float)startY, (float)width, (float)height, 0.0f, 1.0f };
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
  //_copyQueue = _device->CreateCommandQueue(COPY);
  //_copyAllocator = _device->CreateCommandAllocator(COPY);
  //_copyCommandList = _device->CreateCommandList(COPY, _copyAllocator, "Copy List");
  _computeQueue = _device->CreateCommandQueue(COMPUTE);
  _computeAllocator = _device->CreateCommandAllocator(COMPUTE);
  _mouseOverAllocator = _device->CreateCommandAllocator(DIRECT);
  _computeCommandList =
      _device->CreateCommandList(COMPUTE, _computeAllocator, "Compute List");

  _swapChain = _device->CreateSwapChain(_mainQueue, FRAME_COUNT,
                                        _windowDimension.x, _windowDimension.y);
  _frameIndex = _swapChain->CurrentBackBufferIndex();

  _rtvHeap = _device->CreateRenderTargetHeap(32);
  _srvHeap = _device->CreateShaderResourceHeap(128);
  _imGuiSrv = _device->CreateShaderResourceHeap(64);
  _uavHeap = _device->CreateUnorderedAccessHeap(100);
  _cbvHeap = _device->CreateConstantBufferHeap(128);
  _dsvHeap = _device->CreateDepthStencilHeap(2);
  _samplerHeap = _device->CreateSamplerHeap(128);
  _meshesToDraw = {};

  _depthBuffer = _device->CreateDepthBuffer(_windowDimension, "Depth Buffer");
  _mouseOverDepthBuffer = _device->CreateDepthBuffer({_windowDimension.x, _windowDimension.y}, "Mouse Over Depth Buffer");

  auto depthHandle = _dsvHeap->CpuHandleFor(0);
  _device->CreateDepthStencilView(depthHandle, _depthBuffer);
  depthHandle = _dsvHeap->CpuHandleFor(1);
  _device->CreateDepthStencilView(depthHandle, _mouseOverDepthBuffer);

  for (uint32_t x = 0; x < FRAME_COUNT; x++) {
    auto rt = _swapChain->GetBuffer(x);
    auto rtHandle = _rtvHeap->CpuHandleFor(x);
    _device->CreateRenderTargetView(rtHandle, rt);
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

  _imguiAllocator = _device->CreateCommandAllocator(DIRECT);
  _uploadAllocator = _device->CreateCommandAllocator(DIRECT);
  _commandList = _device->CreateCommandList(
      DIRECT, _frames[_frameIndex]->Allocator(), "Main CommandList");
  _imguiCommandList = _device->CreateCommandList(
      DIRECT, _imguiAllocator, "ImGui Command List");
  _mouseOverCommandList = _device->CreateCommandList(
      DIRECT, _mouseOverAllocator, "Mouse Over CommandList");
  _uploadCommandList =
      _device->CreateCommandList(DIRECT, _uploadAllocator, "UploadCommandList");

  for (int x = 0; x < FRAME_COUNT; x++) {
    _mouseOverBuffer[x] = _device->CreateTextureBuffer(
        _windowDimension.x, _windowDimension.y, 4, 1, TextureFormat::RGBA_UINT, "MouseOverBuffer");
    std::stringstream str;
    str << "Frame Buffer" << x;
    _mouseOverRTV[x] = _device->CreateRenderTarget(_mouseOverBuffer[x], str.str());
    auto rtHandle = _rtvHeap->CpuHandleFor(FRAME_COUNT + x);

    _mouseOverCommandList->Transition(_mouseOverBuffer[x], ResourceState::COMMON,
                             ResourceState::COPY_SOURCE);
    _device->CreateRenderTargetView(rtHandle, _mouseOverRTV[x]);
  }

  _mouseOverReadbackBuffer = _device->CreateReadbackBuffer(_windowDimension.x * _windowDimension.y * 4);
  _mouseOverRowPitchActual = (int)(_windowDimension.x * 4);
  _mouseOverRowPitchRequired = (((int)(_windowDimension.x * 4)) + 255) & ~255; 
  _mouseOverPadding = _mouseOverRowPitchActual - _mouseOverRowPitchRequired;

  // Create SRV Texture that allows us to Write the Back Buffer to a shader visible texture
  auto backBufferTex =
      _device->CreateTextureBuffer(_windowDimension.x, _windowDimension.y, 4, 0, Renderer::RGBA, "Back Buffer Texture");
  auto frontBufferTex =
      _device->CreateTextureBuffer(_windowDimension.x, _windowDimension.y, 4, 0, Renderer::RGBA, "Front Buffer Texture");

  _uploadCommandList->Transition(static_pointer_cast<TextureBuffer>(backBufferTex),
      ResourceState::COMMON,
      ResourceState::COPY_DEST);

  auto index = _srvCounter += 2;
  _device->CreateShaderResourceView(backBufferTex,
      _srvHeap->CpuHandleFor(0));
  _device->CreateShaderResourceView(frontBufferTex,
      _srvHeap->CpuHandleFor(1));
  backBufferTex->GPUHandle = _srvHeap->GpuHandleFor(0);
  frontBufferTex->GPUHandle = _srvHeap->GpuHandleFor(1);
  _uploadCommandList->Transition(static_pointer_cast<TextureBuffer>(backBufferTex),
      ResourceState::COPY_DEST,
      ResourceState::PIXEL_SHADER);

  _textures.push_back(backBufferTex);
  _textures.push_back(frontBufferTex);


  //_copyCommandList->Close();
  _commandList->Close();
  _mouseOverCommandList->Close();
  _uploadCommandList->Close();
  _imguiCommandList->Close();
  auto commandLists = {_mouseOverCommandList, _commandList };
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

  //_copyAllocator->Reset();
  //_copyCommandList->Reset(_copyAllocator, nullptr);
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
        _device->CreateUploadBuffer((sizeof(LightBuffer) + 255) & ~255, "Light Buffer");
    _cbAllocator[x]->Reset();

    _device->CreateConstantBufferView(_srvHeap, _lightDataBuffer[x],
                                      _srvHeap->CpuHandleFor(_srvCounter++));
  }

  _entitySelectionBuffer = _device->CreateUploadBuffer(((sizeof(uint32_t) * 4) + 255) & ~255, "EntitySelection Buffer");

  _device->CreateConstantBufferView(_srvHeap, _entitySelectionBuffer,  _srvHeap->CpuHandleFor(_srvCounter) );
  _entitySelectionCBVId = _srvCounter++;

  _cbCommandList = _device->CreateCommandList(DIRECT, _cbAllocator[_frameIndex],
                                              "Constant Buffer CommandList");
  _cbCommandList->Close();

  WaitForGPU(_mainQueue);
  WaitForGPU(_computeQueue);
 // WaitForGPU(_copyQueue);

#if _WIN32
  auto device = static_pointer_cast<D3D12Device>(_device)->Native();
  auto heap = static_pointer_cast<D3D12Heap>(_srvHeap)->Native();
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(reinterpret_cast<SDL_Window *>(_window), &wmInfo);
  HWND hwnd = wmInfo.info.win.window;
  ImGui_ImplWin32_Init(hwnd);
  auto cpuHandle = _srvHeap->CpuHandleFor(_srvCounter);
  auto gpuHandle = _srvHeap->GpuHandleFor(_srvCounter);
  ImGui_ImplDX12_Init(device, FRAME_COUNT, DXGI_FORMAT_R8G8B8A8_UNORM, heap,
      D3D12_CPU_DESCRIPTOR_HANDLE{ cpuHandle->Address() }, D3D12_GPU_DESCRIPTOR_HANDLE{ gpuHandle->Address() });
  _srvCounter++;
#endif
}

auto Interface::CreateAssets() -> void {}

auto Interface::CreateUI(void *context) -> void {}

auto Interface::FillCommandList() -> void {}

auto Interface::MoveToNextFrame() -> void {
  const uint64_t currentFenceValue = _frames[_frameIndex]->FenceValue();
  _mainQueue->Signal(_fence, currentFenceValue);

  //_copyQueue->Signal(_fence, currentFenceValue);

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

auto Interface::FlushGPU() -> void {
    for (int x = 0; x < FRAME_COUNT; x++)
    {
        _mainQueue->Signal(_fence, _frames[x]->FenceValue());
        // Wait until the fence has been processed.
        _fence->SetOnCompletion(_frames[x]->FenceValue(), _fenceEvent);
        _fence->Wait(_frames[x]->FenceValue(), _fenceEvent);
        // Increment the fence value for the current frame.
        auto _ = _frames[x]->IncrementFenceValue();
    }
}

} // namespace Renderer
