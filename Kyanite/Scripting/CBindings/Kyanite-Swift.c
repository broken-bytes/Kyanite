#include "Kyanite-Swift.h"
#include <Windows.h>
typedef void(*TAddRef)(NativeRef* objc);
typedef void(*TRemoveRef)(NativeRef* objc);
typedef ModelInfo(*TLoadModelCPU)(const char* path);
typedef TextureInfo(*TLoadTextureCPU)(const char* path);
typedef NativeRef*(*TLoadShaderGPU)(
    const char* path
);
typedef NativeRef*(*TLoadMaterialGPU)(NativeRef* shader, NativeRef* textures, uint64_t textureCount);
typedef void(*TInit)(uint32_t resolutionX, uint32_t resolutionY, void* window);
typedef void(*TShutdown)();
typedef void(*TSetMaxFrameRate)(uint16_t maxFramerate);
typedef void(*TSetVSync)(bool enabled);
typedef void(*TUpdate)(float frameTime);
typedef void(*TPhysicsUpdate)(float frameTime);
typedef void(*TDrawMesh)(
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
typedef void(*TSetClearColor)(float r, float g, float b, float a);
typedef void(*TSetFogColor)(float r, float g, float b, float a);
typedef void(*TSetFogIntensity)(float intensity);
typedef void(*TSetFogMinDistance)(float distance);
typedef void(*TSetMeshPosition)(NativeRef* ref, float x, float y, float z);
typedef void(*TSetMeshScale)(NativeRef* ref, float x, float y, float z);
typedef void(*TSetMeshRotation)(NativeRef* ref, float x, float y, float z);
typedef void(*TTranslateMesh)(NativeRef* mesh, float x, float y, float z);
typedef void(*TScaleMesh)(NativeRef* mesh, float x, float y, float z);
typedef void(*TRotateMesh)(NativeRef* mesh, float x, float y, float z);
typedef void(*TSetCamera)(    
	float xPos, 
    float yPos, 
    float zPos, 
    float xRotation,
    float yRotation,
    float zRotation);
TAddRef _AddRef;
TRemoveRef _RemoveRef;
TLoadModelCPU _LoadModelCPU;
TLoadTextureCPU _LoadTextureCPU;
TLoadShaderGPU _LoadShaderGPU;
TLoadMaterialGPU _LoadMaterialGPU;
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
void InitCBindings() {
    HANDLE lib = LoadLibrary("./Kyanite-Runtime.dll");
    _AddRef = (TAddRef)GetProcAddress(lib, "AddRef");
    _RemoveRef = (TRemoveRef)GetProcAddress(lib, "RemoveRef");
    _LoadModelCPU = (TLoadModelCPU)GetProcAddress(lib, "LoadModelCPU");
    _LoadTextureCPU = (TLoadTextureCPU)GetProcAddress(lib, "LoadTextureCPU");
    _LoadShaderGPU = (TLoadShaderGPU)GetProcAddress(lib, "LoadShaderGPU");
    _LoadMaterialGPU = (TLoadMaterialGPU)GetProcAddress(lib, "LoadMaterialGPU");
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

}
void AddRef(NativeRef* objc) {
    _AddRef(objc);
}
void RemoveRef(NativeRef* objc) {
    _RemoveRef(objc);
}
ModelInfo LoadModelCPU(const char* path) {
    _LoadModelCPU(path);
}
TextureInfo LoadTextureCPU(const char* path) {
    _LoadTextureCPU(path);
}
NativeRef* LoadShaderGPU(
    const char* path
) {
    _LoadShaderGPU(path
);
}
NativeRef* LoadMaterialGPU(NativeRef* shader, NativeRef* textures, uint64_t textureCount) {
    _LoadMaterialGPU(shader,textures,textureCount);
}
void Init(uint32_t resolutionX, uint32_t resolutionY, void* window) {
    _Init(resolutionX,resolutionY,window);
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
    ) {
    _DrawMesh(mesh,material,xPos,yPos,zPos,xScale,yScale,zScale,xRotation,yRotation,zRotation
);
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
void SetMeshPosition(NativeRef* ref, float x, float y, float z) {
    _SetMeshPosition(ref,x,y,z);
}
void SetMeshScale(NativeRef* ref, float x, float y, float z) {
    _SetMeshScale(ref,x,y,z);
}
void SetMeshRotation(NativeRef* ref, float x, float y, float z) {
    _SetMeshRotation(ref,x,y,z);
}
void TranslateMesh(NativeRef* mesh, float x, float y, float z) {
    _TranslateMesh(mesh,x,y,z);
}
void ScaleMesh(NativeRef* mesh, float x, float y, float z) {
    _ScaleMesh(mesh,x,y,z);
}
void RotateMesh(NativeRef* mesh, float x, float y, float z) {
    _RotateMesh(mesh,x,y,z);
}
void SetCamera(    
	float xPos, 
    float yPos, 
    float zPos, 
    float xRotation,
    float yRotation,
    float zRotation) {
    _SetCamera(xPos,yPos,zPos,xRotation,yRotation,zRotation);
}
