#include <algorithm>
#include <memory>
#include <SDL2/SDL.h>
#include <stduuid/uuid.h>
#include <string.h>
#include "Engine.hxx"
#include "Mesh.hxx"
#include "Rendering/Interface.hxx"
#include "Rendering/Vertex.hxx"
#include "Core/AssetLoader.hxx"
#include "Core/NativeRef.hxx"
#include "Shader.hxx"



std::vector<NativeRef*> _nativeRefs = {};
std::unique_ptr<Renderer::Interface> Interface;

// Internal Helpers

// --- Reference Functions ---
 void AddRef(NativeRef* objc) {
    objc->RefCount++;
 }

 void RemoveRef(NativeRef* objc) {
    objc->RefCount--;

    if(objc->Type != DYNAMIC) {
        // We only want to clear dynamic references. Others stay in memory
        return;
    }
    auto deleter = std::find_if(_nativeRefs.begin(), _nativeRefs.end(), [objc](NativeRef* e) { return strcmp(e->UUID, objc->UUID);});

    if(deleter != _nativeRefs.end()) {
        auto deleteFunc = (void (*)(const char*))objc->Deleter;
        deleteFunc(objc->UUID);
        _nativeRefs.erase(deleter);
    }
 }

DLL_EXPORT void SetRootDir(const char* path) {
    AssetLoader::SetRootDir(path);
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
    info.Data.Lighting = ShaderJSONDataLightingModel::DEFAULT;
    
    info.Data.Input = new ShaderJSONDataInputProp[shader.Description.Props.size()];

    for (auto [it, end, x] = std::tuple{shader.Description.Props.begin(),shader.Description.Props.end(), 0}; it != end; it++, x++){
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
        info.Data.Input[x] = prop;
    }

    auto nameLen = shader.Name.size();
    auto name = new char[nameLen + 1];
    shader.Name.copy(name, nameLen);
    name[nameLen] = '\0';
    info.Data.Name = name;
    info.Data.InputLen = shader.Description.Props.size();

    return info;
}

// Loads a shader and compiles it
DLL_EXPORT NativeRef *LoadShaderGPU(ShaderInfo &info) {
        Renderer::GraphicsShader shader = {};
        shader.Code = info.Data.Code;
        shader.Name = std::string(info.Data.Name);

        shader.Slots = {};

        for (int x = 0; x < info.Data.InputLen; x++) {
                Renderer::GraphicsShaderSlotType type = [&]() {
                  switch (info.Data.Input[x].Type) {
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
                slot.Name = info.Data.Input[x].Name;
                slot.Index = info.Data.Input[x].Slot;
                slot.Type = type;
                shader.Slots.push_back(slot);
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
DLL_EXPORT NativeRef* LoadMaterialGPU(NativeRef* shader) {
    auto index = Interface->CreateMaterial(shader->Identifier);

    auto ref = new NativeRef();
    ref->Identifier = index;
    ref->RefCount = 1;
    ref->Type = STATIC;

    return ref;
}


// --- Commands ---
 void Init(uint32_t resolutionX, uint32_t resolutionY, void* window) {
    Interface = std::make_unique<Renderer::Interface>(resolutionX, resolutionY, window, Renderer::RenderBackendAPI::DirectX12);
 }
 void Shutdown() {}
 void SetMaxFrameRate(uint16_t maxFramerate) {}
 void SetVSync(bool enabled) {}
 void Update(float frameTime) {
    Interface->Update();
    Interface->StartFrame();
    Interface->MidFrame();
    Interface->EndFrame();
 }
 void PhysicsUpdate(float frameTime) {}
 void DrawMesh(
    NativeRef* mesh,
    NativeRef* material,
    MeshDrawInfo info,
    Transform transform
    ) {
        Interface->DrawMesh(mesh->Identifier, material->Identifier, info, {transform.Position.X, transform.Position.Y, transform.Position.Z}, {transform.Rotation.X, transform.Rotation.Y, transform.Rotation.Z}, {transform.Scale.X, transform.Scale.Y, transform.Scale.Z});
    }

 void SetClearColor(float r, float g, float b, float a) {}
 void SetFogColor(float r, float g, float b, float a) {}
 void SetFogIntensity(float intensity) {}
 void SetFogMinDistance(float distance) {}
 void SetMeshPosition(NativeRef* ref, float x, float y, float z) {}
 void SetMeshScale(NativeRef* ref, float x, float y, float z) {}
 void SetMeshRotation(NativeRef* ref, float x, float y, float z) {}
 void TranslateMesh(NativeRef* mesh, float x, float y, float z) {}
 void ScaleMesh(NativeRef* mesh, float x, float y, float z) {}
 void RotateMesh(NativeRef* mesh, float x, float y, float z) {}

 void SetCamera(    
	float xPos, 
    float yPos, 
    float zPos, 
    float xRotation,
    float yRotation,
    float zRotation) {
        Interface->SetCamera({xPos, yPos, zPos}, {xRotation, yRotation, zRotation});
    }