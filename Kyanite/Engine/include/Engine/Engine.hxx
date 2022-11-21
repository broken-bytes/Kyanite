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

struct NativeRef {
    uint32_t RefID;
    uint16_t RefCount;
    void* Data;
} typedef NativeRef;


// --- Reference Functions ---
DLL_EXPORT void AddRef(NativeRef* objc);
DLL_EXPORT void RemoveRef(NativeRef* objc);

// --- Create Functions ---

// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
DLL_EXPORT NativeRef* LoadMeshGPU(const char* path);
// Loads a mesh into CPU memory (RAM)
DLL_EXPORT NativeRef* LoadMeshCPU(
    const char* path,
    float* vertices, 
    size_t vertCount, 
    float* indices, 
    size_t indCount);
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
DLL_EXPORT NativeRef* LoadTextureGPU(const char* path);
// Loads a texture into CPU memory (RAM)
DLL_EXPORT NativeRef* LoadTextureCPU(
    const char* path,
    uint8_t* pixels, 
    size_t pixelCount, 
    uint8_t* channels
    );
// Loads a shader and compiles it 
DLL_EXPORT NativeRef* LoadShader(
    const char* path
    );

// --- Commands ---
DLL_EXPORT void Init(uint32_t resolutionX, uint32_t resolutionY);
DLL_EXPORT void Shutdown();
DLL_EXPORT void SetMaxFrameRate(uint16_t maxFramerate);
DLL_EXPORT void SetVSync(bool enabled);
DLL_EXPORT void Update(float frameTime);
DLL_EXPORT void PhysicsUpdate(float frameTime);
DLL_EXPORT void DrawMesh(
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

#ifdef __cplusplus
}
#endif 