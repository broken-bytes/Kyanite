#include <algorithm>
#include <memory>
#include <SDL2/SDL.h>
#include <stduuid/uuid.h>
#include <string.h>
#include <thread>
#include "Engine.hxx"
#include "Mesh.hxx"
#include "MeshComponent.hxx"
#include "Rendering/Interface.hxx"
#include "Rendering/Vertex.hxx"
#include "Core/AssetLoader.hxx"
#include "Shader.hxx"
#include "ResourceTracker.hxx"

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
#include "glm/ext/quaternion_transform.hpp"

#include "imgui.h"

#define FLECS_CUSTOM_BUILD  // Don't build all addons
#define FLECS_SYSTEM        // Build FLECS_SYSTEM
#define FLECS_META
#define FLECS_META_C
#define FLECS_EXPR
#define FLECS_SNAPSHOT
#define FLECS_LOG
#define FLECS_PIPELINE
#include <flecs.h>


std::unique_ptr<Renderer::Interface> Interface;

ecs_world_t* ECS;
ecs_entity_t Scene;

// Internal Helpers

void SetRootDir(const char* path) {
    AssetLoader::SetRootDir(path);
}

void SetMaterialTexture(NativeRef* material, const char* name, NativeRef* texture) {
    Interface->SetMaterialTexture(material->Identifier, name, texture->Identifier);
}

void SetMaterialPropertyInt(NativeRef* material, const char* name, int value) {

}

void SetMaterialPropertyFloat(NativeRef* material, const char* name, float value) {

}

void SetMaterialPropertyVector2(NativeRef* material, const char* name, float* value) {

}

void SetMaterialPropertyVector3(NativeRef* material, const char* name, float* value) {

}

void SetMaterialPropertyVector4(NativeRef* material, const char* name, float* value) {

}

// --- Create Functions ---

// --- Load Functions ---
DLL_EXPORT ModelInfo LoadModelCPU(const char* path) {
    auto model = AssetLoader::LoadModel(path);

	ModelInfo info;

	MeshInfo* m = new MeshInfo[model.Meshes.size()];
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

DLL_EXPORT void FreeModelCPU(ModelInfo& info) {
    delete[] info.Meshes;
}

// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
DLL_EXPORT NativeRef* LoadMeshGPU(MeshInfo& info) {
    auto index = Interface->UploadMeshData((Vertex*)info.Vertices, info.VerticesCount / 9, info.Indices, info.IndicesCount);
    auto ref = new NativeRef();
    ref->Identifier = index;
    ref->RefCount = 1;
    ref->Type = STATIC;
    ref->Deleter = nullptr;
   std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};
    auto id = gen();
    ref->UUID = uuids::to_string(id).c_str();

    return ref;
}

// Loads a texture into CPU memory (RAM)
DLL_EXPORT TextureInfo LoadTextureCPU(const char* path) {
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

DLL_EXPORT NativeRef* LoadTextureGPU(TextureInfo& info) {
    auto data = Interface->UploadTextureData(info.Levels[0].Data, info.Levels[0].Width, info.Levels[0].Height, 4);
    auto ref = new NativeRef();
    ref->Identifier = data;
    ref->RefCount = 1;
    ref->Type = STATIC;
    ref->Deleter = nullptr;
   std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};
    auto id = gen();
    ref->UUID = uuids::to_string(id).c_str();

    return ref;
}

DLL_EXPORT void FreeTextureCPU(TextureInfo& info) {
    for (int x = 0; x < info.LevelCount; x++) {
		delete[] info.Levels[x].Data;
	}
	delete[] info.Levels;
}

DLL_EXPORT ShaderInfo LoadShaderCPU(const char* path) {
    const char* Name;
    ShaderJSONDataLightingModel Lighting;
    ShaderJSONDataInputProp* Input;
    size_t InputLen;
    const char* Code;
    ShaderInfo info;
    auto shader = AssetLoader::LoadShader(path);

    // Copy code as we cannot pass strings to Swift runtime, so we copy it into char buff
    auto codeLen = shader.Code.size();
    auto code = new char[codeLen + 1];
    shader.Code.copy(code, codeLen);
    code[codeLen] = '\0';
    info.Data.Code = code;

    // Only default lighting for now
    info.Data.Lighting = shader.Description.IsLit ? ShaderJSONDataLightingModel::DEFAULT : ShaderJSONDataLightingModel::UNLIT;

    switch (shader.Description.Format) {
    case AssetLoader::ShaderAssetOutputFormat::RGBA_FLOAT:
                info.Data.Format = SHADER_OUTPUT_RGBA_FLOAT;
                break;
    case AssetLoader::ShaderAssetOutputFormat::RGBA_UINT:
                info.Data.Format = SHADER_OUTPUT_RGBA_UINT;
                break;
    }

    info.Data.Constants = new ShaderJSONDataInputProp[shader.Description.Constants.size()];
    info.Data.ConstantBufferLayout = new ShaderJSONDataInputProp[shader.Description.ConstantBufferLayout.size()];

    for (auto [it, end, x] = std::tuple{shader.Description.Constants.begin(),shader.Description.Constants.end(), 0}; it != end; it++, x++){
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
        char* propName = new char[propNameLen + 1];
        item->Name.copy(propName, propNameLen);
        propName[propNameLen] = '\0';
        prop.Name = propName;
        prop.Type = type;
        prop.Slot = item->Slot;
        info.Data.Constants[x] = prop;
    }

        for (auto [it, end, x] = std::tuple{shader.Description.ConstantBufferLayout.begin(),shader.Description.ConstantBufferLayout.end(), 0}; it != end; it++, x++){
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
        char* propName = new char[propNameLen + 1];
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
    info.Data.ConstantBufferLayoutLen = shader.Description.ConstantBufferLayout.size();

    return info;
}

// Loads a shader and compiles it
DLL_EXPORT NativeRef *LoadShaderGPU(ShaderInfo &info) {
        Renderer::GraphicsShader shader = {};
        shader.Code = info.Data.Code;
        shader.Name = std::string(info.Data.Name);
        shader.IsLit = info.Data.Lighting == ShaderJSONDataLightingModel::DEFAULT ? true : false;

        switch(info.Data.Format) {
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


        auto index = Interface->UploadShaderData(shader);

        auto ref = new NativeRef();
        ref->Identifier = index;
        ref->RefCount = 1;
        ref->Type = STATIC;
        ref->Deleter = nullptr;
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size>{};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::mt19937 generator(seq);
        uuids::uuid_random_generator gen{generator};
        auto id = gen();
        ref->UUID = uuids::to_string(id).c_str();

        return ref;
}

// Creates a new material in the renderpipeline and returns its ref
DLL_EXPORT NativeRef* LoadMaterialGPU(const char* name, NativeRef* shader) {
    auto index = Interface->CreateMaterial(name, shader->Identifier);

    auto ref = new NativeRef();
    ref->Identifier = index;
    ref->RefCount = 1;
    ref->Type = STATIC;

    return ref;
}


// --- Commands ---
 void Init(uint32_t resolutionX, uint32_t resolutionY, void* window, void* ctx, void* style) {
  ECS = ecs_init();
  ecs_set_threads(ECS, std::thread::hardware_concurrency());
  Scene = ecs_new_id(ECS);
  auto context = ImGui::CreateContext();
  ImGui::SetCurrentContext(context);
  ImGuiIO &io = ImGui::GetIO();
  // Enable Gamepad Controls
  ImGui::StyleColorsDark();
    Interface = std::make_unique<Renderer::Interface>(resolutionX, resolutionY, window, &context, Renderer::RenderBackendAPI::DirectX12);
 }
 
 void Shutdown() {}
 void SetMaxFrameRate(uint16_t maxFramerate) {}
 void SetVSync(bool enabled) {}
 void Update(float frameTime) {
    Interface->Update();
    Interface->StartFrame();
    ImGui::Begin("Entities");
    auto termIt = ecs_term_t { ecs_pair(EcsChildOf, Scene) };
    ecs_iter_t it = ecs_term_iter(ECS, &termIt);
    //ecs_iter_poly(ECS, ECS, &it, NULL);
    while (ecs_iter_next(&it)) {
      for (int i = 0; i < it.count; i++) {
        auto text = ecs_get_name(it.world, it.entities[i]);
        if(text != nullptr) {
          ImGui::Text(text); // Need the Name here
        }
      }
    }
    ImGui::End();
    Interface->MidFrame();
    Interface->EndFrame();
 }
 void PhysicsUpdate(float frameTime) {}
 void DrawMesh(
    uint64_t entityId,
    NativeRef* mesh,
    NativeRef* material,
    MeshDrawInfo info,
    Transform transform
    ) {
      glm::vec3 position = glm::vec3(transform.Position.X, transform.Position.Y, transform.Position.Z);
      glm::vec3 scale = glm::vec3(transform.Scale.X, transform.Scale.Y, transform.Scale.Z);
      glm::quat qRotation = glm::quat(transform.Rotation.Q, transform.Rotation.X, transform.Rotation.Y, transform.Rotation.Z);
        Interface->DrawMesh(entityId, mesh->Identifier, material->Identifier, info, position, qRotation, scale);
    }

 void SetClearColor(float r, float g, float b, float a) {}
 void SetFogColor(float r, float g, float b, float a) {}
 void SetFogIntensity(float intensity) {}
 void SetFogMinDistance(float distance) {}

 void SetCamera(    
	float xPos, 
    float yPos, 
    float zPos, 
    float xRotation,
    float yRotation,
    float zRotation) {
        Interface->SetCamera({xPos, yPos, zPos}, {xRotation, yRotation, zRotation});
    }

void SetCursorPosition(uint32_t x, uint32_t y) {
  Interface->SetCursorPosition({x, y});
}

uint32_t GetMouseOverEntityId(uint32_t x, uint32_t y) {
    return Interface->ReadMouseOverData(x, y);
}

// Entity functions


uint64_t CreateEntity(const char* name) {
  ecs_entity_t e = ecs_new_w_pair(ECS, EcsChildOf, Scene);
  ecs_set_name(ECS, e, name);
  return e;
}

uint64_t RegisterComponent(uint64_t size, uint8_t alignment, const char* uuid) {
  ecs_component_desc_t desc = {};
  desc.type = {};
  desc.type.size = size;
  desc.type.alignment = alignment;
  desc.type.name = uuid;

  return ecs_component_init(ECS, &desc);
}

uint64_t AddComponent(uint64_t entity, uint64_t id, uint64_t size, void* data) {
  ecs_set_id(ECS, entity, id, size, data);
  return 0;
}

const void* GetComponent(uint64_t entity, uint64_t id) {
  return ecs_get_id(ECS, entity, id);
}