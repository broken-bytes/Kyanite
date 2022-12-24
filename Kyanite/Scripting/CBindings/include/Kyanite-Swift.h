#pragma once
#include <stdbool.h>



#ifdef __cplusplus 
extern "C" {
#endif

struct Vertex {
    float Position[4];
    float Normal[3];
    float UV[2];
    float Color[4];
} typedef Vertex;

#ifdef __cplusplus 
}
#endif



#include <stdint.h>


#ifdef __cplusplus 
extern "C" {
#endif

enum MeshDrawInfoFlags {
    MESH_DRAW_INFO_FLAGS_DRAW_NONE = 0,
    MESH_DRAW_INFO_FLAGS_DRAW_OUTLINE = 1
};


struct MeshDrawInfo {
    uint8_t Flags;
    float OutlineWidth;
    float OutlineColor[4];
} typedef MeshDrawInfo;

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

#ifdef _WIN32

#else

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

struct Quaternion {
  float Q;
  float X;
  float Y;
  float Z;
} typedef Quaternion;

struct Color {
  float R;
  float G;
  float B;
  float A;
} typedef Color;

struct MeshDrawInfo;

struct Transform {
  Vector3 Position;
  Quaternion Rotation;
  Vector3 Scale;
} typedef Transform;

struct MeshInfo {
  float *Vertices;
  int VerticesCount;
  uint32_t *Indices;
  int IndicesCount;
} typedef MeshInfo;

struct ModelInfo {
  MeshInfo *Meshes;
  int MeshCount;
} typedef ModelInfo;

struct TextureLevelInfo {
  uint8_t *Data;
  uint16_t Width;
  uint16_t Height;
} typedef TextureLevelInfo;

struct TextureInfo {
  TextureLevelInfo *Levels;
  uint8_t LevelCount;
  uint8_t Channels;
} typedef TextureInfo;

enum ShaderJSONDataLightingModel {
    DEFAULT,
    UNLIT
} typedef ShaderJSONDataLightingModel;

enum ShaderJSONDataStageType {
    VERTEX,
    PIXEL,
    MESH
} typedef ShaderJSONDataStageType;

enum ShaderJSONDataOutputFormat {
  SHADER_OUTPUT_RGBA_FLOAT = 0,
  SHADER_OUTPUT_RGBA_UINT = 1
} typedef ShaderJSONDataOutputFormat;

enum ShaderJSONDataInputPropType {
    SHADER_PROP_INT,
    SHADER_PROP_BOOL,
    SHADER_PROP_FLOAT,
    SHADER_PROP_FLOAT2,
    SHADER_PROP_FLOAT3,
    SHADER_PROP_FLOAT4,
    SHADER_PROP_TEXTURE
} typedef ShaderJSONDataInputPropType;

struct ShaderJSONDataInputProp {
    const char* Name;
    ShaderJSONDataInputPropType Type;
    uint8_t Slot;
};

// Internal Helpers
struct ShaderJSONData {
    const char* Name;
    ShaderJSONDataOutputFormat Format;
    ShaderJSONDataLightingModel Lighting;
    ShaderJSONDataInputProp* ConstantBufferLayout;
    uint64_t ConstantBufferLayoutLen;
    ShaderJSONDataInputProp* Constants;
    uint64_t ConstantsLen;
    const char* Code;
};

struct ShaderInfo {
  ShaderJSONData Data;
} typedef ShaderInfo;

// --- Reference Functions ---
 void AddRef(NativeRef *objc);
 void RemoveRef(NativeRef *objc);
// --- Create Functions ---

// --- Setter Functions ---
 void SetRootDir(const char* path);
 void SetMaterialTexture(NativeRef* material, const char* name, NativeRef* texture);
 void SetMaterialPropertyInt(NativeRef* material, const char* name, int value);
 void SetMaterialPropertyFloat(NativeRef* material, const char* name, float value);
 void SetMaterialPropertyVector2(NativeRef* material,const char* name, float* value);
 void SetMaterialPropertyVector3(NativeRef* material, const char* name, float* value);
 void SetMaterialPropertyVector4(NativeRef* material, const char* name, float* value);

// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if
// not supported)
 NativeRef *LoadMeshGPU(MeshInfo &info);
// Loads a mesh into CPU memory (RAM)
 ModelInfo LoadModelCPU(const char *path);
 void FreeModelCPU(ModelInfo &info);
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU
// if not supported)
 NativeRef *LoadTextureGPU(TextureInfo &info);
// Loads a texture into CPU memory (RAM)
 TextureInfo LoadTextureCPU(const char *path);
 void FreeTextureCPU(TextureInfo &info);
 ShaderInfo LoadShaderCPU(const char *path);
// Loads a shader and compiles it
 NativeRef *LoadShaderGPU(ShaderInfo &info);

// Creates a new material in the renderpipeline and returns its ref
 NativeRef *LoadMaterialGPU(const char* name, NativeRef *shader);

// --- Commands ---
 void Init(uint32_t resolutionX, uint32_t resolutionY, void *window);
 void Shutdown();
 void SetMaxFrameRate(uint16_t maxFramerate);
 void SetVSync(bool enabled);
 void Update(float frameTime);
 void PhysicsUpdate(float frameTime);
 void DrawMesh(uint64_t entityId, NativeRef *mesh, NativeRef *material,
                         MeshDrawInfo info, Transform transform);

 void SetClearColor(float r, float g, float b, float a);
 void SetFogColor(float r, float g, float b, float a);
 void SetFogIntensity(float intensity);
 void SetFogMinDistance(float distance);
 void SetMeshPosition(NativeRef *ref, float x, float y, float z);
 void SetMeshScale(NativeRef *ref, float x, float y, float z);
 void SetMeshRotation(NativeRef *ref, float x, float y, float z);
 void TranslateMesh(NativeRef *mesh, float x, float y, float z);
 void ScaleMesh(NativeRef *mesh, float x, float y, float z);
 void RotateMesh(NativeRef *mesh, float x, float y, float z);
 void SetCamera(float xPos, float yPos, float zPos, float xRotation,
                          float yRotation, float zRotation);
 void SetCursorPosition(uint32_t x, uint32_t y);
 uint32_t GetMouseOverEntityId(uint32_t x, uint32_t y);
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
