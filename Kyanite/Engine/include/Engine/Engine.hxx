// #SwiftImport
#pragma once

#include <cstdint>
#include <stdint.h>

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
    size_t ConstantBufferLayoutLen;
    ShaderJSONDataInputProp* Constants;
    size_t ConstantsLen;
    const char* Code;
};

struct ShaderInfo {
  ShaderJSONData Data;
} typedef ShaderInfo;

// --- Reference Functions ---
DLL_EXPORT void AddRef(NativeRef *objc);
DLL_EXPORT void RemoveRef(NativeRef *objc);
// --- Create Functions ---

// --- Setter Functions ---
DLL_EXPORT void SetRootDir(const char* path);
DLL_EXPORT void SetMaterialTexture(NativeRef* material, const char* name, NativeRef* texture);
DLL_EXPORT void SetMaterialPropertyInt(NativeRef* material, const char* name, int value);
DLL_EXPORT void SetMaterialPropertyFloat(NativeRef* material, const char* name, float value);
DLL_EXPORT void SetMaterialPropertyVector2(NativeRef* material,const char* name, float* value);
DLL_EXPORT void SetMaterialPropertyVector3(NativeRef* material, const char* name, float* value);
DLL_EXPORT void SetMaterialPropertyVector4(NativeRef* material, const char* name, float* value);

// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if
// not supported)
DLL_EXPORT NativeRef *LoadMeshGPU(MeshInfo &info);
// Loads a mesh into CPU memory (RAM)
DLL_EXPORT ModelInfo LoadModelCPU(const char *path);
DLL_EXPORT void FreeModelCPU(ModelInfo &info);
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU
// if not supported)
DLL_EXPORT NativeRef *LoadTextureGPU(TextureInfo &info);
// Loads a texture into CPU memory (RAM)
DLL_EXPORT TextureInfo LoadTextureCPU(const char *path);
DLL_EXPORT void FreeTextureCPU(TextureInfo &info);
DLL_EXPORT ShaderInfo LoadShaderCPU(const char *path);
// Loads a shader and compiles it
DLL_EXPORT NativeRef *LoadShaderGPU(ShaderInfo &info);

// Creates a new material in the renderpipeline and returns its ref
DLL_EXPORT NativeRef *LoadMaterialGPU(const char* name, NativeRef *shader);

// --- Commands ---
DLL_EXPORT void Init(uint32_t resolutionX, uint32_t resolutionY, void *window);
DLL_EXPORT void Shutdown();
DLL_EXPORT void SetMaxFrameRate(uint16_t maxFramerate);
DLL_EXPORT void SetVSync(bool enabled);
DLL_EXPORT void Update(float frameTime);
DLL_EXPORT void PhysicsUpdate(float frameTime);
DLL_EXPORT void DrawMesh(uint64_t entityId, NativeRef *mesh, NativeRef *material,
                         MeshDrawInfo info, Transform transform);

DLL_EXPORT void SetClearColor(float r, float g, float b, float a);
DLL_EXPORT void SetFogColor(float r, float g, float b, float a);
DLL_EXPORT void SetFogIntensity(float intensity);
DLL_EXPORT void SetFogMinDistance(float distance);
DLL_EXPORT void SetMeshPosition(NativeRef *ref, float x, float y, float z);
DLL_EXPORT void SetMeshScale(NativeRef *ref, float x, float y, float z);
DLL_EXPORT void SetMeshRotation(NativeRef *ref, float x, float y, float z);
DLL_EXPORT void TranslateMesh(NativeRef *mesh, float x, float y, float z);
DLL_EXPORT void ScaleMesh(NativeRef *mesh, float x, float y, float z);
DLL_EXPORT void RotateMesh(NativeRef *mesh, float x, float y, float z);
DLL_EXPORT void SetCamera(float xPos, float yPos, float zPos, float xRotation,
                          float yRotation, float zRotation);
DLL_EXPORT void SetCursorPosition(uint32_t x, uint32_t y);
DLL_EXPORT uint32_t GetMouseOverEntityId(uint32_t x, uint32_t y);

// Entity system

enum COMPONENT_PROP_TYPE {
  COMPONENT_PROP_TYPE_INT,
  COMPONENT_PROP_TYPE_UINT,
  COMPONENT_PROP_TYPE_FLOAT,
  COMPONENT_PROP_TYPE_BOOL,
  COMPONENT_PROP_TYPE_STRING
} typedef COMPONENT_PROP_TYPE;

DLL_EXPORT uint64_t CreateEntity();
DLL_EXPORT uint64_t RegisterComponent(uint64_t size, uint8_t alignment, const char* uuid);
DLL_EXPORT uint64_t AddComponent(uint64_t entity, uint64_t id, uint64_t size, void* data);
DLL_EXPORT const void* GetComponent(uint64_t entity, uint64_t id);

#ifdef __cplusplus
}
#endif