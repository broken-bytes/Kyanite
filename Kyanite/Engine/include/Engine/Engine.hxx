// #SwiftImport
#pragma once

#include <cstdint>


#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT 
#endif

#ifdef __cplusplus
extern "C" {
#endif 

struct NativeRef;

struct Vector3 {
    float X;
    float Y;
    float Z;
} typedef Vector3;

struct Vector4 {
    float X;
    float Y;
    float Z;
    float W;
} typedef Vector4;

struct Color {
    float R;
    float G;
    float B;
    float A;
} typedef Color;

struct MeshDrawInfo;

struct Transform {
    Vector3 Position;
    Vector3 Rotation;
    Vector3 Scale;
} typedef Transform;

struct MeshInfo {
		float* Vertices;
		int VerticesCount;
		uint32_t* Indices;
		int IndicesCount;
	} typedef MeshInfo;


	struct ModelInfo {
		MeshInfo* Meshes;
		int MeshCount;
	} typedef ModelInfo;

    struct TextureLevelInfo {
		uint8_t* Data;
		uint16_t Width;
        uint16_t Height;
	} typedef TextureLevelInfo;


	struct TextureInfo {
		TextureLevelInfo* Levels;
		uint8_t LevelCount;
		uint8_t Channels;
	} typedef TextureInfo;

    struct ShaderInfo {
		const char* Code;
        uint32_t Size; 
	} typedef ShaderInfo;

// --- Reference Functions ---
DLL_EXPORT void AddRef(NativeRef* objc);
DLL_EXPORT void RemoveRef(NativeRef* objc);

// --- Create Functions ---

// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
DLL_EXPORT NativeRef* LoadMeshGPU(MeshInfo& info);
// Loads a mesh into CPU memory (RAM)
DLL_EXPORT ModelInfo LoadModelCPU(const char* path);
DLL_EXPORT void FreeModelCPU(ModelInfo& info);
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
DLL_EXPORT NativeRef* LoadTextureGPU(TextureInfo& info);
// Loads a texture into CPU memory (RAM)
DLL_EXPORT TextureInfo LoadTextureCPU(const char* path);
DLL_EXPORT void FreeTextureCPU(TextureInfo& info);
DLL_EXPORT ShaderInfo LoadShaderCPU(const char* path);
// Loads a shader and compiles it 
DLL_EXPORT NativeRef* LoadShaderGPU(
    ShaderInfo& info
);

// Creates a new material in the renderpipeline and returns its ref
DLL_EXPORT NativeRef* LoadMaterialGPU(NativeRef* shader, NativeRef* textures, size_t textureCount);


// --- Commands ---
DLL_EXPORT void Init(uint32_t resolutionX, uint32_t resolutionY, void* window);
DLL_EXPORT void Shutdown();
DLL_EXPORT void SetMaxFrameRate(uint16_t maxFramerate);
DLL_EXPORT void SetVSync(bool enabled);
DLL_EXPORT void Update(float frameTime);
DLL_EXPORT void PhysicsUpdate(float frameTime);
DLL_EXPORT void DrawMesh(
    NativeRef* mesh,
    NativeRef* material,
    MeshDrawInfo info,
    Transform transform
);

DLL_EXPORT void SetClearColor(float r, float g, float b, float a);
DLL_EXPORT void SetFogColor(float r, float g, float b, float a);
DLL_EXPORT void SetFogIntensity(float intensity);
DLL_EXPORT void SetFogMinDistance(float distance);
DLL_EXPORT void SetMeshPosition(NativeRef* ref, float x, float y, float z);
DLL_EXPORT void SetMeshScale(NativeRef* ref, float x, float y, float z);
DLL_EXPORT void SetMeshRotation(NativeRef* ref, float x, float y, float z);
DLL_EXPORT void TranslateMesh(NativeRef* mesh, float x, float y, float z);
DLL_EXPORT void ScaleMesh(NativeRef* mesh, float x, float y, float z);
DLL_EXPORT void RotateMesh(NativeRef* mesh, float x, float y, float z);
DLL_EXPORT void SetCamera(    
	float xPos, 
    float yPos, 
    float zPos, 
    float xRotation,
    float yRotation,
    float zRotation);

#ifdef __cplusplus
}
#endif 