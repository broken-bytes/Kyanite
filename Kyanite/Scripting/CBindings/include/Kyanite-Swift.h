#pragma once
#include <stdbool.h>



#ifdef __cplusplus 
extern "C" {
#endif

struct Vertex {
    float Position[3];
    float UV[2];
    float Normal[3];
} typedef Vertex;

#ifdef __cplusplus 
}
#endif



#include <stdint.h>


#ifdef __cplusplus 
extern "C" {
#endif

struct Mesh {
   Vertex* Vertices;
   uint64_t NumVertices;
   uint32_t* Indices;
   uint64_t NumIndices;
} typedef Mesh;

#ifdef __cplusplus 
}
#endif


#ifdef _WIN32

#else
 
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
 void AddRef(NativeRef* objc);
 void RemoveRef(NativeRef* objc);

// --- Create Functions ---

// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
 NativeRef* LoadMeshGPU(const char* path);
// Loads a mesh into CPU memory (RAM)
 NativeRef* LoadMeshCPU(
    const char* path,
    float* vertices, 
    uint64_t vertCount, 
    float* indices, 
    uint64_t indCount);
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
 NativeRef* LoadTextureGPU(const char* path);
// Loads a texture into CPU memory (RAM)
 NativeRef* LoadTextureCPU(
    const char* path,
    uint8_t* pixels, 
    uint64_t pixelCount, 
    uint8_t* channels
    );
// Loads a shader and compiles it 
 NativeRef* LoadShader(
    const char* path
    );

// --- Commands ---
 void Init(uint32_t resolutionX, uint32_t resolutionY);
 void Shutdown();
 void SetMaxFrameRate(uint16_t maxFramerate);
 void SetVSync(bool enabled);
 void Update(float frameTime);
 void PhysicsUpdate(float frameTime);
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
    );

 void SetClearColor(float r, float g, float b, float a);
 void SetFogColor(float r, float g, float b, float a);
 void SetFogIntensity(float intensity);
 void SetFogMinDistance(float distance);
 void SetMeshPosition(NativeRef* ref, float x, float y, float z);
 void SetMeshScale(NativeRef* ref, float x, float y, float z);
 void SetMeshRotation(NativeRef* ref, float x, float y, float z);
 void TranslateMesh(NativeRef* mesh, float x, float y, float z);
 void ScaleMesh(NativeRef* mesh, float x, float y, float z);
 void RotateMesh(NativeRef* mesh, float x, float y, float z);

#ifdef __cplusplus
}
#endif #ifdef __cplusplus
extern "C" {
#endif
 void InitCBindings();
#ifdef __cplusplus
}
#endif
