// #SwiftImport
#pragma once

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
    bool InstancingEnabled;
    ShaderJSONDataInputProp* ConstantBufferLayout;
    size_t ConstantBufferLayoutLen;
    ShaderJSONDataInputProp* Constants;
    size_t ConstantsLen;
    const char* Code;
};

struct ShaderInfo {
  ShaderJSONData Data;
} typedef ShaderInfo;

#pragma region ENGINE_API
DLL_EXPORT void Engine_Init(uint32_t resolutionX, uint32_t resolutionY, void *window);
DLL_EXPORT void Engine_Shutdown();
DLL_EXPORT void Engine_Update(float frameTime);
DLL_EXPORT void Engine_StartRender();
DLL_EXPORT void Engine_EndRender();
DLL_EXPORT void Engine_Resize(uint32_t width, uint32_t height);
DLL_EXPORT uint64_t Engine_GetOutputTexture();
DLL_EXPORT uint64_t Engine_LoadMeshGPU(MeshInfo& info);
DLL_EXPORT ModelInfo Engine_LoadModelCPU(const char* path);
DLL_EXPORT void Engine_FreeModelCPU(ModelInfo& info);
DLL_EXPORT uint64_t Engine_LoadTextureGPU(TextureInfo& info);
DLL_EXPORT TextureInfo Engine_LoadTextureCPU(const char* path);
DLL_EXPORT void Engine_FreeTextureCPU(TextureInfo& info);
DLL_EXPORT ShaderInfo Engine_LoadShaderCPU(const char* path);
DLL_EXPORT uint64_t Engine_LoadShaderGPU(ShaderInfo& info);
DLL_EXPORT uint64_t Engine_LoadMaterialGPU(const char* name, uint64_t shader);
DLL_EXPORT void Engine_SetMaxFrameRate(uint16_t maxFramerate);
DLL_EXPORT void Engine_SetVSync(bool enabled);
DLL_EXPORT void Engine_SetRootDir(const char* path);
DLL_EXPORT void Engine_SetCursorPosition(uint32_t x, uint32_t y);
DLL_EXPORT void Engine_SetCamera(float xPos, float yPos, float zPos, float xRotation,
                          float yRotation, float zRotation);
#pragma endregion

#pragma region INPUT_API
DLL_EXPORT uint8_t Input_GetKeyboardButton(uint16_t code);
DLL_EXPORT uint8_t Input_GetMouseButton(uint8_t button);
DLL_EXPORT void Input_GetMouseMovement(float* x, float* y);
DLL_EXPORT void Input_GetMousePosition(uint32_t* x, uint32_t* y);
#pragma endregion

#pragma region RENDER_API
DLL_EXPORT void Renderer_DrawMesh(uint64_t entityId, uint64_t mesh, uint64_t material, MeshDrawInfo info, Transform transform);
DLL_EXPORT void Renderer_DrawLine(float* from, float* to, float* color);
//DLL_EXPORT void Renderer_SetClearColor(float r, float g, float b, float a);
//DLL_EXPORT void Renderer_SetFogColor(float r, float g, float b, float a);
//DLL_EXPORT void Renderer_SetFogIntensity(float intensity);
//DLL_EXPORT void Renderer_SetFogMinDistance(float distance);
#pragma endregion

#pragma region SHADER_API
DLL_EXPORT void Shader_SetMaterialTexture(uint64_t material, const char* name, uint64_t texture);
DLL_EXPORT void Shader_SetMaterialPropertyInt(uint64_t material, const char* name, int value);
DLL_EXPORT void Shader_SetMaterialPropertyFloat(uint64_t material, const char* name, float value);
DLL_EXPORT void Shader_SetMaterialPropertyVector2(uint64_t material,const char* name, float* value);
DLL_EXPORT void Shader_SetMaterialPropertyVector3(uint64_t material, const char* name, float* value);
DLL_EXPORT void Shader_SetMaterialPropertyVector4(uint64_t material, const char* name, float* value);
#pragma endregion

#pragma region ENTITY_API
DLL_EXPORT uint64_t ECS_CreateEntity(const char* name);
DLL_EXPORT void ECS_DeleteEntity(uint64_t id);
DLL_EXPORT uint64_t ECS_RegisterComponent(uint64_t size, uint8_t alignment, const char* uuid);
DLL_EXPORT uint64_t ECS_AddComponent(uint64_t entity, uint64_t id, uint64_t size, void* data);
DLL_EXPORT const void* ECS_GetComponent(uint64_t entity, uint64_t id);
DLL_EXPORT uint32_t ECS_GetMouseOverEntityId(uint32_t x, uint32_t y);
DLL_EXPORT uint64_t ECS_RegisterSystem(const char* name, void* system, bool multiThreaded, uint64_t* componentIds, size_t numComponents);
DLL_EXPORT void* ECS_GetComponentData(void* iterator, size_t size, uint8_t index, size_t* count);
DLL_EXPORT void ECS_GetSystemDelta(void* iterator, float* delta);
DLL_EXPORT uint64_t ECS_GetSystemFromIter(void* iterator);
#pragma endregion

#pragma region IMGUI_API
DLL_EXPORT void IMGUI_NotifyMouseDown(uint8_t button);
DLL_EXPORT void IMGUI_NotifyMouseUp(uint8_t button);
DLL_EXPORT void IMGUI_NotifyMouseMove(int32_t x, int32_t y);
DLL_EXPORT void IMGUI_StartWindow(const char* name);
DLL_EXPORT void IMGUI_EndWindow();
DLL_EXPORT void IMGUI_StartChild();
DLL_EXPORT void IMGUI_EndChild();
DLL_EXPORT void IMGUI_DrawText(const char* text);
DLL_EXPORT void IMGUI_DrawFloatField(const char* title, float* value);
DLL_EXPORT void IMGUI_DrawFloat2Field(const char* title, float* value);
DLL_EXPORT void IMGUI_DrawFloat3Field(const char* title, float* value);
DLL_EXPORT void IMGUI_DrawIntField(const char* title, int* value);
DLL_EXPORT void IMGUI_DrawColorPicker(const char* title, float* color);
#pragma endregion

#ifdef __cplusplus
}
#endif
