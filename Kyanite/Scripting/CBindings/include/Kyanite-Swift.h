#pragma once
#include <stdbool.h>



#ifdef __cplusplus 
extern "C" {
#endif

struct Vertex {
    float Position[4];
    float Normal[3];
    float UV[2];
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


#include <stdint.h>

struct NativeRef;

#ifdef _WIN32

#else
 
#endif

#ifdef __cplusplus
extern "C" {
#endif 

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
 void AddRef(NativeRef* objc);
 void RemoveRef(NativeRef* objc);

// --- Create Functions ---

// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
 NativeRef* LoadMeshGPU(MeshInfo& info);
// Loads a mesh into CPU memory (RAM)
 ModelInfo LoadModelCPU(const char* path);
 void FreeModelCPU(ModelInfo& info);
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if not supported)
 NativeRef* LoadTextureGPU(TextureInfo& info);
// Loads a texture into CPU memory (RAM)
 TextureInfo LoadTextureCPU(const char* path);
 void FreeTextureCPU(TextureInfo& info);

// Loads a shader and compiles it 
 NativeRef* LoadShaderGPU(
    const char* path
);

// Creates a new material in the renderpipeline and returns its ref
 NativeRef* LoadMaterialGPU(NativeRef* shader, NativeRef* textures, uint64_t textureCount);


// --- Commands ---
 void Init(uint32_t resolutionX, uint32_t resolutionY, void* window);
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
 void SetCamera(    
	float xPos, 
    float yPos, 
    float zPos, 
    float xRotation,
    float yRotation,
    float zRotation);

#ifdef __cplusplus
}
#endif 


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif 
enum RefType {
    STATIC,
    DYNAMIC,
};

struct NativeRef {
    const char* UUID;
    uint16_t RefCount;
    uint64_t Identifier;
    RefType Type;
    // Function pointer to function called upon delete of this Pointer
    void* Deleter;

} typedef NativeRef;

#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
 void InitCBindings();
#ifdef __cplusplus
}
#endif
