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


#pragma region EXPORTED_API
//NOTE - We are using void* Pointers instead of std::shared_ptr<TrackedResource> for the C-style exported API. 
// Reasons: 
// - Swift can only handle C-style types(for now)
// - We cannot export C++ types from within a C API
// - C++ name mangling is a mess
// The void* pointer is still std::shared_ptr<TrackedResource> but cast to void* so Swift can keep a ref to it.

#pragma region RUNTIME_API
DLL_EXPORT void Init(uint32_t resolutionX, uint32_t resolutionY, void *window);
DLL_EXPORT void Shutdown();
DLL_EXPORT void Update(float frameTime);
DLL_EXPORT void StartRender();
DLL_EXPORT void EndRender();
DLL_EXPORT void Resize(uint32_t width, uint32_t height);
#pragma endregion

#pragma region CONFIG_API
DLL_EXPORT void SetMaxFrameRate(uint16_t maxFramerate);
DLL_EXPORT void SetVSync(bool enabled);
DLL_EXPORT void SetRootDir(const char* path);
DLL_EXPORT void SetCursorPosition(uint32_t x, uint32_t y);
DLL_EXPORT void SetCamera(float xPos, float yPos, float zPos, float xRotation,
                          float yRotation, float zRotation);
#pragma endregion

#pragma region SHADER_API
DLL_EXPORT void SetMaterialTexture(uint64_t material, const char* name, uint64_t texture);
DLL_EXPORT void SetMaterialPropertyInt(uint64_t material, const char* name, int value);
DLL_EXPORT void SetMaterialPropertyFloat(uint64_t material, const char* name, float value);
DLL_EXPORT void SetMaterialPropertyVector2(uint64_t material,const char* name, float* value);
DLL_EXPORT void SetMaterialPropertyVector3(uint64_t material, const char* name, float* value);
DLL_EXPORT void SetMaterialPropertyVector4(uint64_t material, const char* name, float* value);
#pragma endregion

#pragma region ENTITY_API
DLL_EXPORT uint64_t CreateEntity(const char* name);
DLL_EXPORT uint64_t RegisterComponent(uint64_t size, uint8_t alignment, const char* uuid);
DLL_EXPORT uint64_t AddComponent(uint64_t entity, uint64_t id, uint64_t size, void* data);
DLL_EXPORT const void* GetComponent(uint64_t entity, uint64_t id);
DLL_EXPORT uint32_t GetMouseOverEntityId(uint32_t x, uint32_t y);
DLL_EXPORT uint64_t RegisterSystem(const char* name, void* system, uint64_t* componentIds, size_t numComponents);
DLL_EXPORT void* GetComponentData(void* iterator, size_t size, uint8_t index, size_t* count);
DLL_EXPORT void GetSystemDelta(void* iterator, float* delta);
#pragma endregion

#pragma region IMGUI_API
DLL_EXPORT void IMGUI_NotifyMouseDown(uint8_t button);
DLL_EXPORT void IMGUI_NotifyMouseUp(uint8_t button);
DLL_EXPORT void IMGUI_NotifyMouseMove(int32_t x, int32_t y);
DLL_EXPORT void IMGUI_StartWindow(const char* name);
DLL_EXPORT void IMGUI_EndWindow();
DLL_EXPORT void IMGUI_StartChild();
DLL_EXPORT void IMGUI_EndChild();
DLL_EXPORT void IMGUI_DrawFloatField(const char* title, float* value);
DLL_EXPORT void IMGUI_DrawFloat2Field(const char* title, float* value);
DLL_EXPORT void IMGUI_DrawFloat3Field(const char* title, float* value);
DLL_EXPORT void IMGUI_DrawIntField(const char* title, int* value);
DLL_EXPORT void IMGUI_DrawColorPicker(const char* title, float* color);
#pragma endregion

#pragma endregion
DLL_EXPORT void SetClearColor(float r, float g, float b, float a);
DLL_EXPORT void SetFogColor(float r, float g, float b, float a);
DLL_EXPORT void SetFogIntensity(float intensity);
DLL_EXPORT void SetFogMinDistance(float distance);

#ifdef __cplusplus
}
#endif

#pragma region INTERNAL_API
// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if
// not supported)
uint64_t LoadMeshGPU(MeshInfo &info);
// Loads a mesh into CPU memory (RAM)
ModelInfo LoadModelCPU(const char *path);

void FreeModelCPU(ModelInfo &info);
// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU
// if not supported)
uint64_t  LoadTextureGPU(TextureInfo &info);
// Loads a texture into CPU memory (RAM)
TextureInfo LoadTextureCPU(const char *path);
void FreeTextureCPU(TextureInfo &info);
ShaderInfo LoadShaderCPU(const char *path);
// Loads a shader and compiles it
uint64_t LoadShaderGPU(ShaderInfo &info);
// Creates a new material in the renderpipeline and returns its ref
uint64_t LoadMaterialGPU(const char* name, uint64_t shader);
void DrawMesh(uint64_t entityId, uint64_t mesh, uint64_t material, MeshDrawInfo info, Transform transform);

#pragma endregion
