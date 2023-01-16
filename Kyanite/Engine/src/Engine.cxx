#include "Engine.hxx"
#include "Core/AssetLoader.hxx"
#include "Mesh.hxx"
#include "MeshComponent.hxx"
#include "Rendering/Interface.hxx"
#include "Rendering/Vertex.hxx"
#include "ResourceTracker.hxx"
#include "Shader.hxx"
#include <SDL2/SDL.h>
#include <algorithm>
#include <memory>
#include <stduuid/uuid.h>
#include <string.h>
#include <thread>

#include "glm/common.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/common.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/mat4x4.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "glm/vec4.hpp"

#include "imgui.h"

#define FLECS_CUSTOM_BUILD // Don't build all addons
#define FLECS_SYSTEM
#define FLECS_META
#define FLECS_META_C
#define FLECS_EXPR
#define FLECS_SNAPSHOT
#define FLECS_LOG
#define FLECS_PIPELINE
#include <flecs.h>

struct EngineInstance {
  std::unique_ptr<Renderer::Interface> Renderer;
  ecs_world_t *ECS;
  ecs_entity_t Scene;
};

std::unique_ptr<EngineInstance> Instance;

#pragma region EXPORTED_API
// NOTE - We are using void* Pointers instead of
// std::shared_ptr<TrackedResource> for the C-style exported API.
//  Reasons:
//  - Swift can only handle C-style types(for now)
//  - We cannot export C++ types from within a C API
//  - C++ name mangling is a mess
//  The void* pointer is still std::shared_ptr<TrackedResource> but cast to
//  void* so Swift can keep a ref to it.

#pragma region RUNTIME_API
void Init(uint32_t resolutionX, uint32_t resolutionY, void *window) {
  Instance->ECS = ecs_init();
  ecs_set_threads(Instance->ECS, std::thread::hardware_concurrency());
  Instance->Scene = ecs_new_id(Instance->ECS);
  auto context = ImGui::CreateContext();
  ImGui::SetCurrentContext(context);
  ImGuiIO &io = ImGui::GetIO();
  // Enable Gamepad Controls
  ImGui::StyleColorsDark();
  Instance->Renderer = std::make_unique<Renderer::Interface>(
      resolutionX, resolutionY, window, Renderer::RenderBackendAPI::DirectX12);
}

void Shutdown() {}

void Update(float frameTime) {
  Instance->Renderer->Update();
  Instance->Renderer->StartFrame();
  ImGui::Begin("Entities");
  auto termIt = ecs_term_t{ecs_pair(EcsChildOf, Instance->Scene)};
  ecs_iter_t it = ecs_term_iter(Instance->ECS, &termIt);
  // ecs_iter_poly(ECS, ECS, &it, NULL);
  while (ecs_iter_next(&it)) {
    for (int i = 0; i < it.count; i++) {
      auto text = ecs_get_name(it.world, it.entities[i]);
      if (text != nullptr) {
        ImGui::Text(text); // Need the Name here
      }
    }
  }
  ImGui::End();
  Instance->Renderer->MidFrame();
  Instance->Renderer->EndFrame();
}

void PhysicsUpdate(float frameTime) {}
#pragma endregion

#pragma region CONFIG_API
void SetMaxFrameRate(uint16_t maxFramerate) {}

void SetVSync(bool enabled) {}

void SetRootDir(const char *path) { AssetLoader::SetRootDir(path); }

void SetCursorPosition(uint32_t x, uint32_t y) {}

void SetCamera(float xPos, float yPos, float zPos, float xRotation,
               float yRotation, float zRotation) {
  Instance->Renderer->SetCamera({xPos, yPos, zPos},
                                {xRotation, yRotation, zRotation});
}
#pragma endregion

#pragma region SHADER_API
void SetMaterialTexture(uint64_t material, const char *name, uint64_t texture) {
  Instance->Renderer->SetMaterialTexture(material, name, texture);
}

void SetMaterialPropertyInt(uint64_t material, const char *name, int value) {}

void SetMaterialPropertyFloat(uint64_t material, const char *name,
                              float value) {}

void SetMaterialPropertyVector2(uint64_t material, const char *name,
                                float *value) {}

void SetMaterialPropertyVector3(uint64_t material, const char *name,
                                float *value) {}

void SetMaterialPropertyVector4(uint64_t material, const char *name,
                                float *value) {}

#pragma endregion

#pragma region ENTITY_API
uint64_t CreateEntity(const char *name) {
  ecs_entity_t e = ecs_new_w_pair(Instance->ECS, EcsChildOf, Instance->Scene);
  ecs_set_name(Instance->ECS, e, name);
  return e;
}

uint64_t RegisterComponent(uint64_t size, uint8_t alignment, const char *uuid) {
  ecs_component_desc_t desc = {};
  desc.type = {};
  desc.type.size = size;
  desc.type.alignment = alignment;
  desc.type.name = uuid;

  return ecs_component_init(Instance->ECS, &desc);
}

uint64_t AddComponent(uint64_t entity, uint64_t id, uint64_t size, void *data) {
  ecs_set_id(Instance->ECS, entity, id, size, data);
  return 0;
}

const void *GetComponent(uint64_t entity, uint64_t id) {
  return ecs_get_id(Instance->ECS, entity, id);
}

uint32_t GetMouseOverEntityId(uint32_t x, uint32_t y) {
  return Instance->Renderer->ReadMouseOverData(x, y);
}
#pragma endregion

#pragma region RENDERING_API
#pragma endregion

#pragma endregion

void SetClearColor(float r, float g, float b, float a) {}

void SetFogColor(float r, float g, float b, float a) {}

void SetFogIntensity(float intensity) {}

void SetFogMinDistance(float distance) {}

#pragma region INTERNAL_API
// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if
// not supported)
uint64_t LoadMeshGPU(MeshInfo &info) {
  auto index = Instance->Renderer->UploadMeshData(
      (Vertex *)info.Vertices, info.VerticesCount / 9, info.Indices,
      info.IndicesCount);
  return index;
}

// Loads a mesh into CPU memory (RAM)
ModelInfo LoadModelCPU(const char *path) {
  auto model = AssetLoader::LoadModel(path);

  ModelInfo info;

  MeshInfo *m = new MeshInfo[model.Meshes.size()];
  for (int x = 0; x < model.Meshes.size(); x++) {
    MeshInfo mesh = {};
    mesh.Vertices = new float[model.Meshes[x].Vertices.size()];
    for (int y = 0; y < model.Meshes[x].Vertices.size(); y++) {
      mesh.Vertices[y] = model.Meshes[x].Vertices[y];
    }
    mesh.VerticesCount = model.Meshes[x].Vertices.size();

    mesh.Indices = new uint32_t[model.Meshes[x].Indicies.size()];
    for (int y = 0; y < model.Meshes[x].Indicies.size(); y++) {
      mesh.Indices[y] = model.Meshes[x].Indicies[y];
    }
    mesh.IndicesCount = model.Meshes[x].Indicies.size();
    m[x] = mesh;
  }
  info.Meshes = m;
  info.MeshCount = model.Meshes.size();

  return info;
}

void FreeModelCPU(ModelInfo &info) { delete[] info.Meshes; }

// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU
// if not supported)
uint64_t LoadTextureGPU(TextureInfo &info) {
  return Instance->Renderer->UploadTextureData(info.Levels[0].Data, info.Levels[0].Width,
                                      info.Levels[0].Height, 4);
}

// Loads a texture into CPU memory (RAM)
TextureInfo LoadTextureCPU(const char *path) {
  auto texture = AssetLoader::LoadTexture(path);

  TextureInfo info;
  info.LevelCount = texture.Levels.size();
  info.Levels = new TextureLevelInfo[info.LevelCount];
  info.Channels = texture.Channels;
  for (int x = 0; x < info.LevelCount; x++) {
    // Always assume 4 bytes per pixel for now
    size_t bytes = texture.Levels[x].Width * texture.Levels[x].Height * 4;
    info.Levels[x].Data = texture.Levels[x].Data;
    info.Levels[x].Width = texture.Levels[x].Width;
    info.Levels[x].Height = texture.Levels[x].Height;
  }

  return info;
}

void FreeTextureCPU(TextureInfo &info) {
  for (int x = 0; x < info.LevelCount; x++) {
    delete[] info.Levels[x].Data;
  }

  delete[] info.Levels;
}

ShaderInfo LoadShaderCPU(const char *path) {
  const char *Name;
  ShaderJSONDataLightingModel Lighting;
  ShaderJSONDataInputProp *Input;
  size_t InputLen;
  const char *Code;
  ShaderInfo info;
  auto shader = AssetLoader::LoadShader(path);

  // Copy code as we cannot pass strings to Swift runtime, so we copy it into
  // char buff
  auto codeLen = shader.Code.size();
  auto code = new char[codeLen + 1];
  shader.Code.copy(code, codeLen);
  code[codeLen] = '\0';
  info.Data.Code = code;

  // Only default lighting for now
  info.Data.Lighting = shader.Description.IsLit
                           ? ShaderJSONDataLightingModel::DEFAULT
                           : ShaderJSONDataLightingModel::UNLIT;

  switch (shader.Description.Format) {
  case AssetLoader::ShaderAssetOutputFormat::RGBA_FLOAT:
    info.Data.Format = SHADER_OUTPUT_RGBA_FLOAT;
    break;
  case AssetLoader::ShaderAssetOutputFormat::RGBA_UINT:
    info.Data.Format = SHADER_OUTPUT_RGBA_UINT;
    break;
  }

  info.Data.Constants =
      new ShaderJSONDataInputProp[shader.Description.Constants.size()];
  info.Data.ConstantBufferLayout =
      new ShaderJSONDataInputProp[shader.Description.ConstantBufferLayout
                                      .size()];

  for (auto [it, end, x] = std::tuple{shader.Description.Constants.begin(),
                                      shader.Description.Constants.end(), 0};
       it != end; it++, x++) {
    auto item = it;
    ShaderJSONDataInputPropType type = [&, item]() {
      switch (item->Type) {
      case AssetLoader::ShaderAssetDescriptionPropType::TEXTURE:
        return ShaderJSONDataInputPropType::SHADER_PROP_TEXTURE;
      case AssetLoader::ShaderAssetDescriptionPropType::FLOAT:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT;
      case AssetLoader::ShaderAssetDescriptionPropType::VECTOR2:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT2;
      case AssetLoader::ShaderAssetDescriptionPropType::VECTOR3:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT3;
      case AssetLoader::ShaderAssetDescriptionPropType::VECTOR4:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT4;
      case AssetLoader::ShaderAssetDescriptionPropType::BOOL:
        return ShaderJSONDataInputPropType::SHADER_PROP_BOOL;
      case AssetLoader::ShaderAssetDescriptionPropType::INT:
        return ShaderJSONDataInputPropType::SHADER_PROP_INT;
      default:
        throw std::runtime_error("Unknown shader prop type");
      }
    }();
    ShaderJSONDataInputProp prop = {};
    size_t propNameLen = item->Name.length();
    char *propName = new char[propNameLen + 1];
    item->Name.copy(propName, propNameLen);
    propName[propNameLen] = '\0';
    prop.Name = propName;
    prop.Type = type;
    prop.Slot = item->Slot;
    info.Data.Constants[x] = prop;
  }

  for (auto [it, end, x] =
           std::tuple{shader.Description.ConstantBufferLayout.begin(),
                      shader.Description.ConstantBufferLayout.end(), 0};
       it != end; it++, x++) {
    auto item = it;
    ShaderJSONDataInputPropType type = [&, item]() {
      switch (item->Type) {
      case AssetLoader::ShaderAssetDescriptionPropType::TEXTURE:
        return ShaderJSONDataInputPropType::SHADER_PROP_TEXTURE;
      case AssetLoader::ShaderAssetDescriptionPropType::FLOAT:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT;
      case AssetLoader::ShaderAssetDescriptionPropType::VECTOR2:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT2;
      case AssetLoader::ShaderAssetDescriptionPropType::VECTOR3:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT3;
      case AssetLoader::ShaderAssetDescriptionPropType::VECTOR4:
        return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT4;
      case AssetLoader::ShaderAssetDescriptionPropType::BOOL:
        return ShaderJSONDataInputPropType::SHADER_PROP_BOOL;
      case AssetLoader::ShaderAssetDescriptionPropType::INT:
        return ShaderJSONDataInputPropType::SHADER_PROP_INT;
      default:
        throw std::runtime_error("Unknown shader prop type");
      }
    }();
    ShaderJSONDataInputProp prop = {};
    size_t propNameLen = item->Name.length();
    char *propName = new char[propNameLen + 1];
    item->Name.copy(propName, propNameLen);
    propName[propNameLen] = '\0';
    prop.Name = propName;
    prop.Type = type;
    prop.Slot = item->Slot;
    info.Data.ConstantBufferLayout[x] = prop;
  }

  auto nameLen = shader.Name.size();
  auto name = new char[nameLen + 1];
  shader.Name.copy(name, nameLen);
  name[nameLen] = '\0';
  info.Data.Name = name;
  info.Data.ConstantsLen = shader.Description.Constants.size();
  info.Data.ConstantBufferLayoutLen =
      shader.Description.ConstantBufferLayout.size();

  return info;
}

// Loads a shader and compiles it
uint64_t LoadShaderGPU(ShaderInfo &info) {
  Renderer::GraphicsShader shader = {};
  shader.Code = info.Data.Code;
  shader.Name = std::string(info.Data.Name);
  shader.IsLit =
      info.Data.Lighting == ShaderJSONDataLightingModel::DEFAULT ? true : false;

  switch (info.Data.Format) {
  case SHADER_OUTPUT_RGBA_FLOAT:
    shader.Format = Renderer::TextureFormat::RGBA;
    break;
  case SHADER_OUTPUT_RGBA_UINT:
    shader.Format = Renderer::TextureFormat::RGBA_UINT;
    break;
  default:
    break;
  }

  shader.Constants = {};
  shader.ConstantBufferLayout = {};

  for (int x = 0; x < info.Data.ConstantsLen; x++) {
    Renderer::GraphicsShaderSlotType type = [&]() {
      switch (info.Data.Constants[x].Type) {
      case SHADER_PROP_TEXTURE:
        return Renderer::GraphicsShaderSlotType::TEXTURE;
      case SHADER_PROP_FLOAT:
        return Renderer::GraphicsShaderSlotType::FLOAT;
      case SHADER_PROP_FLOAT2:
        return Renderer::GraphicsShaderSlotType::VECTOR2;
      case SHADER_PROP_FLOAT3:
        return Renderer::GraphicsShaderSlotType::VECTOR3;
      case SHADER_PROP_FLOAT4:
        return Renderer::GraphicsShaderSlotType::VECTOR4;
      case SHADER_PROP_INT:
        return Renderer::GraphicsShaderSlotType::INT;
      case SHADER_PROP_BOOL:
        return Renderer::GraphicsShaderSlotType::BOOL;
      default:
        throw std::runtime_error("Unknown shader prop type");
      }
    }();
    Renderer::GraphicsShaderSlot slot = {};
    slot.Name = info.Data.Constants[x].Name;
    slot.Index = info.Data.Constants[x].Slot;
    slot.Type = type;
    shader.Constants.push_back(slot);
  }

  for (int x = 0; x < info.Data.ConstantBufferLayoutLen; x++) {
    Renderer::GraphicsShaderSlotType type = [&]() {
      switch (info.Data.ConstantBufferLayout[x].Type) {
      case SHADER_PROP_TEXTURE:
        return Renderer::GraphicsShaderSlotType::TEXTURE;
      case SHADER_PROP_FLOAT:
        return Renderer::GraphicsShaderSlotType::FLOAT;
      case SHADER_PROP_FLOAT2:
        return Renderer::GraphicsShaderSlotType::VECTOR2;
      case SHADER_PROP_FLOAT3:
        return Renderer::GraphicsShaderSlotType::VECTOR3;
      case SHADER_PROP_FLOAT4:
        return Renderer::GraphicsShaderSlotType::VECTOR4;
      case SHADER_PROP_INT:
        return Renderer::GraphicsShaderSlotType::INT;
      case SHADER_PROP_BOOL:
        return Renderer::GraphicsShaderSlotType::BOOL;
      default:
        throw std::runtime_error("Unknown shader prop type");
      }
    }();
    Renderer::GraphicsShaderSlot slot = {};
    slot.Name = info.Data.ConstantBufferLayout[x].Name;
    slot.Index = info.Data.ConstantBufferLayout[x].Slot;
    slot.Type = type;
    shader.ConstantBufferLayout.push_back(slot);
  }

  auto index = Instance->Renderer->UploadShaderData(shader);

  return index;
}

// Creates a new material in the renderpipeline and returns its ref
uint64_t LoadMaterialGPU(const char *name, uint64_t shader) {
  return Instance->Renderer->CreateMaterial(name, shader);
}

void DrawMesh(uint64_t entityId, uint64_t mesh, uint64_t material,
              MeshDrawInfo info, Transform transform) {
  glm::vec3 position = glm::vec3(transform.Position.X, transform.Position.Y,
                                 transform.Position.Z);
  glm::vec3 scale =
      glm::vec3(transform.Scale.X, transform.Scale.Y, transform.Scale.Z);
  glm::quat qRotation = glm::quat(transform.Rotation.Q, transform.Rotation.X,
                                  transform.Rotation.Y, transform.Rotation.Z);
  Instance->Renderer->DrawMesh(entityId, mesh, material, info, position,
                               qRotation, scale);
}

#pragma endregion
