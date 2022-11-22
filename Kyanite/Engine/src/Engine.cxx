#include <algorithm>
#include <memory>
#include <SDL2/SDL.h>
#include <stduuid/uuid.h>
#include <string.h>
#include "Engine.hxx"
#include "Rendering/Interface.hxx"
#include "Rendering/Vertex.hxx"
#include "Core/AssetLoader.hxx"



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
    auto index = Interface->UploadMeshData((Vertex*)info.Vertices, info.VerticesCount / sizeof(Vertex), info.Indices, info.IndicesCount);
    auto ref = new NativeRef();
    ref->Data = nullptr;
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
DLL_EXPORT void FreeTextureCPU(TextureInfo& info) {
    for (int x = 0; x < info.LevelCount; x++) {
		delete[] info.Levels[x].Data;
	}
	delete[] info.Levels;
}
// Loads a shader and compiles it 
DLL_EXPORT NativeRef* LoadShaderGPU(
    const char* path
) {
    auto shader = AssetLoader::LoadShader(path);
    Interface->UploadShaderData(shader.Code);

        auto ref = new NativeRef();
    ref->Data = nullptr;
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
    Interface->EndFrame();
 }
 void PhysicsUpdate(float frameTime) {}
 void DrawMesh(
    NativeRef* mesh,
    NativeRef* material,
    float xPos, 
    float yPos, 
    float zPos, 
    float xScale, 
    float yScale, 
    float zScale,
    float xRotation,
    float yRotation,
    float zRotation
    ) {}

 void SetClearColor(float r, float g, float b, float a) {
    
 }
 void SetFogColor(float r, float g, float b, float a) {}
 void SetFogIntensity(float intensity) {}
 void SetFogMinDistance(float distance) {}
 void SetMeshPosition(NativeRef* ref, float x, float y, float z) {}
 void SetMeshScale(NativeRef* ref, float x, float y, float z) {}
 void SetMeshRotation(NativeRef* ref, float x, float y, float z) {}
 void TranslateMesh(NativeRef* mesh, float x, float y, float z) {}
 void ScaleMesh(NativeRef* mesh, float x, float y, float z) {}
 void RotateMesh(NativeRef* mesh, float x, float y, float z) {}