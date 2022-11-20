#pragma once

#include <cstddef>
#include <cstdint>

#ifdef _WIN32
#define DLL_EXPORT __declspec( dllexport )
#else
#define DLL_EXPORT 
#endif

#ifdef __cplusplus
extern "C" {
#endif 

enum LoaderType {
    CPU,
    GPU
};

struct NativeRef {
    uint32_t RefID;
    uint16_t RefCount;
} typedef NativeRef;


// --- Reference Functions ---
DLL_EXPORT auto AddRef(NativeRef* objc) -> void;
DLL_EXPORT auto RemoveRef(NativeRef* objc) -> void;

// --- Create Functions ---
DLL_EXPORT auto MakeLoader(LoaderType type) -> NativeRef*;

// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
DLL_EXPORT auto LoadMeshGPU(const char* path) -> NativeRef*;
// Loads a mesh into CPU memory (RAM)
DLL_EXPORT auto LoadMeshCPU(
    const char* path,
    float* vertices, 
    size_t vertCount, 
    float* indices, 
    size_t indCount) -> NativeRef*;
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
DLL_EXPORT auto LoadTextureGPU(const char* path) -> NativeRef*;
// Loads a texture into CPU memory (RAM)
DLL_EXPORT auto LoadTextureCPU(
    const char* path,
    uint8_t* pixels, 
    size_t pixelCount, 
    uint8_t* channels
    ) -> NativeRef*;
// Loads a shader and compiles it 
DLL_EXPORT auto LoadShader(
    const char* path
    ) -> NativeRef*;

// --- Commands ---
DLL_EXPORT auto Init(uint32_t resolutionX, uint32_t resolutionY) -> void;
DLL_EXPORT auto Shutdown() -> void;
DLL_EXPORT auto SetMaxFrameRate(uint16_t maxFramerate);
DLL_EXPORT auto SetVSync(bool enabled) -> void;
DLL_EXPORT auto Update(float frameTime) -> void;
DLL_EXPORT auto PhysicsUpdate(float frameTime) -> void;
DLL_EXPORT auto DrawMesh(
    NativeRef* mesh,
    float xPos, 
    float yPos, 
    float zPos, 
    float xScale, 
    float yScale, 
    float zScale,
    float xRotation,
    float yRotation,
    float zRotation
    ) -> void;

DLL_EXPORT auto SetClearColor(float r, float g, float b, float a) -> void;
DLL_EXPORT auto SetFogColor(float r, float g, float b, float a) -> void;
DLL_EXPORT auto SetFogIntensity(float intensity) -> void;
DLL_EXPORT auto SetFogMinDistance(float distance) -> void;
DLL_EXPORT auto SetMeshPosition(NativeRef* ref, float x, float y, float z) -> void;
DLL_EXPORT auto SetMeshScale(NativeRef* ref, float x, float y, float z) -> void;
DLL_EXPORT auto SetMeshRotation(NativeRef* ref, float x, float y, float z) -> void;
DLL_EXPORT auto TranslateMesh(NativeRef* mesh, float x, float y, float z) -> void;
DLL_EXPORT auto ScaleMesh(NativeRef* mesh, float x, float y, float z) -> void;
DLL_EXPORT auto RotateMesh(NativeRef* mesh, float x, float y, float z) -> void;

#ifdef __cplusplus
}
#endif 