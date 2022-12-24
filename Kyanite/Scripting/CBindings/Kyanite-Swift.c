#include "Kyanite-Swift.h"
#include <Windows.h>
typedef void(*TAddRef)(NativeRef *objc);
typedef void(*TRemoveRef)(NativeRef *objc);
typedef void(*TSetRootDir)(const char* path);
typedef void(*TSetMaterialTexture)(NativeRef* material, const char* name, NativeRef* texture);
typedef void(*TSetMaterialPropertyInt)(NativeRef* material, const char* name, int value);
typedef void(*TSetMaterialPropertyFloat)(NativeRef* material, const char* name, float value);
typedef void(*TSetMaterialPropertyVector2)(NativeRef* material,const char* name, float* value);
typedef void(*TSetMaterialPropertyVector3)(NativeRef* material, const char* name, float* value);
typedef void(*TSetMaterialPropertyVector4)(NativeRef* material, const char* name, float* value);
typedef ModelInfo(*TLoadModelCPU)(const char *path);
typedef TextureInfo(*TLoadTextureCPU)(const char *path);
typedef ShaderInfo(*TLoadShaderCPU)(const char *path);
typedef void(*TInit)(uint32_t resolutionX, uint32_t resolutionY, void *window);
typedef void(*TShutdown)();
typedef void(*TSetMaxFrameRate)(uint16_t maxFramerate);
typedef void(*TSetVSync)(bool enabled);
typedef void(*TUpdate)(float frameTime);
typedef void(*TPhysicsUpdate)(float frameTime);
typedef void(*TDrawMesh)(uint64_t entityId, NativeRef *mesh, NativeRef *material,
                         MeshDrawInfo info, Transform transform);
typedef void(*TSetClearColor)(float r, float g, float b, float a);
typedef void(*TSetFogColor)(float r, float g, float b, float a);
typedef void(*TSetFogIntensity)(float intensity);
typedef void(*TSetFogMinDistance)(float distance);
typedef void(*TSetMeshPosition)(NativeRef *ref, float x, float y, float z);
typedef void(*TSetMeshScale)(NativeRef *ref, float x, float y, float z);
typedef void(*TSetMeshRotation)(NativeRef *ref, float x, float y, float z);
typedef void(*TTranslateMesh)(NativeRef *mesh, float x, float y, float z);
typedef void(*TScaleMesh)(NativeRef *mesh, float x, float y, float z);
typedef void(*TRotateMesh)(NativeRef *mesh, float x, float y, float z);
typedef void(*TSetCamera)(float xPos, float yPos, float zPos, float xRotation,
                          float yRotation, float zRotation);
typedef void(*TSetCursorPosition)(uint32_t x, uint32_t y);
typedef uint32_t(*TGetMouseOverEntityId)(uint32_t x, uint32_t y);
TAddRef _AddRef;
TRemoveRef _RemoveRef;
TSetRootDir _SetRootDir;
TSetMaterialTexture _SetMaterialTexture;
TSetMaterialPropertyInt _SetMaterialPropertyInt;
TSetMaterialPropertyFloat _SetMaterialPropertyFloat;
TSetMaterialPropertyVector2 _SetMaterialPropertyVector2;
TSetMaterialPropertyVector3 _SetMaterialPropertyVector3;
TSetMaterialPropertyVector4 _SetMaterialPropertyVector4;
TLoadModelCPU _LoadModelCPU;
TLoadTextureCPU _LoadTextureCPU;
TLoadShaderCPU _LoadShaderCPU;
TInit _Init;
TShutdown _Shutdown;
TSetMaxFrameRate _SetMaxFrameRate;
TSetVSync _SetVSync;
TUpdate _Update;
TPhysicsUpdate _PhysicsUpdate;
TDrawMesh _DrawMesh;
TSetClearColor _SetClearColor;
TSetFogColor _SetFogColor;
TSetFogIntensity _SetFogIntensity;
TSetFogMinDistance _SetFogMinDistance;
TSetMeshPosition _SetMeshPosition;
TSetMeshScale _SetMeshScale;
TSetMeshRotation _SetMeshRotation;
TTranslateMesh _TranslateMesh;
TScaleMesh _ScaleMesh;
TRotateMesh _RotateMesh;
TSetCamera _SetCamera;
TSetCursorPosition _SetCursorPosition;
TGetMouseOverEntityId _GetMouseOverEntityId;
void InitCBindings() {
    HANDLE lib = LoadLibrary("./Kyanite-Runtime.dll");
    _AddRef = (TAddRef)GetProcAddress(lib, "AddRef");
    _RemoveRef = (TRemoveRef)GetProcAddress(lib, "RemoveRef");
    _SetRootDir = (TSetRootDir)GetProcAddress(lib, "SetRootDir");
    _SetMaterialTexture = (TSetMaterialTexture)GetProcAddress(lib, "SetMaterialTexture");
    _SetMaterialPropertyInt = (TSetMaterialPropertyInt)GetProcAddress(lib, "SetMaterialPropertyInt");
    _SetMaterialPropertyFloat = (TSetMaterialPropertyFloat)GetProcAddress(lib, "SetMaterialPropertyFloat");
    _SetMaterialPropertyVector2 = (TSetMaterialPropertyVector2)GetProcAddress(lib, "SetMaterialPropertyVector2");
    _SetMaterialPropertyVector3 = (TSetMaterialPropertyVector3)GetProcAddress(lib, "SetMaterialPropertyVector3");
    _SetMaterialPropertyVector4 = (TSetMaterialPropertyVector4)GetProcAddress(lib, "SetMaterialPropertyVector4");
    _LoadModelCPU = (TLoadModelCPU)GetProcAddress(lib, "LoadModelCPU");
    _LoadTextureCPU = (TLoadTextureCPU)GetProcAddress(lib, "LoadTextureCPU");
    _LoadShaderCPU = (TLoadShaderCPU)GetProcAddress(lib, "LoadShaderCPU");
    _Init = (TInit)GetProcAddress(lib, "Init");
    _Shutdown = (TShutdown)GetProcAddress(lib, "Shutdown");
    _SetMaxFrameRate = (TSetMaxFrameRate)GetProcAddress(lib, "SetMaxFrameRate");
    _SetVSync = (TSetVSync)GetProcAddress(lib, "SetVSync");
    _Update = (TUpdate)GetProcAddress(lib, "Update");
    _PhysicsUpdate = (TPhysicsUpdate)GetProcAddress(lib, "PhysicsUpdate");
    _DrawMesh = (TDrawMesh)GetProcAddress(lib, "DrawMesh");
    _SetClearColor = (TSetClearColor)GetProcAddress(lib, "SetClearColor");
    _SetFogColor = (TSetFogColor)GetProcAddress(lib, "SetFogColor");
    _SetFogIntensity = (TSetFogIntensity)GetProcAddress(lib, "SetFogIntensity");
    _SetFogMinDistance = (TSetFogMinDistance)GetProcAddress(lib, "SetFogMinDistance");
    _SetMeshPosition = (TSetMeshPosition)GetProcAddress(lib, "SetMeshPosition");
    _SetMeshScale = (TSetMeshScale)GetProcAddress(lib, "SetMeshScale");
    _SetMeshRotation = (TSetMeshRotation)GetProcAddress(lib, "SetMeshRotation");
    _TranslateMesh = (TTranslateMesh)GetProcAddress(lib, "TranslateMesh");
    _ScaleMesh = (TScaleMesh)GetProcAddress(lib, "ScaleMesh");
    _RotateMesh = (TRotateMesh)GetProcAddress(lib, "RotateMesh");
    _SetCamera = (TSetCamera)GetProcAddress(lib, "SetCamera");
    _SetCursorPosition = (TSetCursorPosition)GetProcAddress(lib, "SetCursorPosition");
    _GetMouseOverEntityId = (TGetMouseOverEntityId)GetProcAddress(lib, "GetMouseOverEntityId");

}
void AddRef(NativeRef *objc) {
    _AddRef(*objc);
}
void RemoveRef(NativeRef *objc) {
    _RemoveRef(*objc);
}
void SetRootDir(const char* path) {
    _SetRootDir(path);
}
void SetMaterialTexture(NativeRef* material, const char* name, NativeRef* texture) {
    _SetMaterialTexture(material,name,texture);
}
void SetMaterialPropertyInt(NativeRef* material, const char* name, int value) {
    _SetMaterialPropertyInt(material,name,value);
}
void SetMaterialPropertyFloat(NativeRef* material, const char* name, float value) {
    _SetMaterialPropertyFloat(material,name,value);
}
void SetMaterialPropertyVector2(NativeRef* material,const char* name, float* value) {
    _SetMaterialPropertyVector2(material,name,value);
}
void SetMaterialPropertyVector3(NativeRef* material, const char* name, float* value) {
    _SetMaterialPropertyVector3(material,name,value);
}
void SetMaterialPropertyVector4(NativeRef* material, const char* name, float* value) {
    _SetMaterialPropertyVector4(material,name,value);
}
ModelInfo LoadModelCPU(const char *path) {
    _LoadModelCPU(*path);
}
TextureInfo LoadTextureCPU(const char *path) {
    _LoadTextureCPU(*path);
}
ShaderInfo LoadShaderCPU(const char *path) {
    _LoadShaderCPU(*path);
}
void Init(uint32_t resolutionX, uint32_t resolutionY, void *window) {
    _Init(resolutionX,resolutionY,*window);
}
void Shutdown() {
    _Shutdown();
}
void SetMaxFrameRate(uint16_t maxFramerate) {
    _SetMaxFrameRate(maxFramerate);
}
void SetVSync(bool enabled) {
    _SetVSync(enabled);
}
void Update(float frameTime) {
    _Update(frameTime);
}
void PhysicsUpdate(float frameTime) {
    _PhysicsUpdate(frameTime);
}
void DrawMesh(uint64_t entityId, NativeRef *mesh, NativeRef *material,
                         MeshDrawInfo info, Transform transform) {
    _DrawMesh(entityId,*mesh,*material,info,transform);
}
void SetClearColor(float r, float g, float b, float a) {
    _SetClearColor(r,g,b,a);
}
void SetFogColor(float r, float g, float b, float a) {
    _SetFogColor(r,g,b,a);
}
void SetFogIntensity(float intensity) {
    _SetFogIntensity(intensity);
}
void SetFogMinDistance(float distance) {
    _SetFogMinDistance(distance);
}
void SetMeshPosition(NativeRef *ref, float x, float y, float z) {
    _SetMeshPosition(*ref,x,y,z);
}
void SetMeshScale(NativeRef *ref, float x, float y, float z) {
    _SetMeshScale(*ref,x,y,z);
}
void SetMeshRotation(NativeRef *ref, float x, float y, float z) {
    _SetMeshRotation(*ref,x,y,z);
}
void TranslateMesh(NativeRef *mesh, float x, float y, float z) {
    _TranslateMesh(*mesh,x,y,z);
}
void ScaleMesh(NativeRef *mesh, float x, float y, float z) {
    _ScaleMesh(*mesh,x,y,z);
}
void RotateMesh(NativeRef *mesh, float x, float y, float z) {
    _RotateMesh(*mesh,x,y,z);
}
void SetCamera(float xPos, float yPos, float zPos, float xRotation,
                          float yRotation, float zRotation) {
    _SetCamera(xPos,yPos,zPos,xRotation,yRotation,zRotation);
}
void SetCursorPosition(uint32_t x, uint32_t y) {
    _SetCursorPosition(x,y);
}
uint32_t GetMouseOverEntityId(uint32_t x, uint32_t y) {
    _GetMouseOverEntityId(x,y);
}
