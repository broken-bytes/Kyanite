#include "rendering/Bridge_Rendering.h"
#include "rendering/Rendering.hxx"
#include "rendering/Shader.hxx"

#include <imgui.h>
#include <imgui_internal.h>

namespace rendering = kyanite::engine::rendering;

void Rendering_Init(NativePointer window, NativePointer imGuiContext) {
	rendering::Init(window, reinterpret_cast<ImGuiContext*>(imGuiContext));
}

void Rendering_Shutdown() {
	rendering::Shutdown();
}

void Rendering_PreFrame() {
	rendering::PreFrame();
}

void Rendering_Update(float deltaTime) {
	rendering::Update(deltaTime);
}

void Rendering_PostFrame() {
	rendering::PostFrame();
}

// Graphics pipeline functions
uint64_t Rendering_CreateVertexBuffer(NativePointer vertices, size_t length) {
	return rendering::CreateVertexBuffer(vertices, length);
}

void Rendering_UpdateVertexBuffer(uint64_t vertexBufferId, NativePointer vertices, size_t length) {

}

void Rendering_FreeVertexBuffer(uint64_t vertexBufferId) {

}

void Rendering_LoadVertexBuffer(uint64_t vertexBufferId, NativePointer* vertices, size_t* length) {

}

uint64_t Rendering_CreateIndexBuffer(NativePointer indices, size_t length) {
	// TODO: Implement
	return rendering::CreateIndexBuffer(reinterpret_cast<uint32_t*>(indices), length);
}

void Rendering_UpdateIndexBuffer(uint64_t indexBufferId, NativePointer indices) {
	// TODO: Implement
	return;
}

/**
* @brief Frees an index buffer
* @param indexBufferId The id of the index buffer to free
*/
EXPORTED void Rendering_FreeIndexBuffer(uint64_t indexBufferId);

uint64_t Rendering_CreateShader(const char* shader, uint8_t shaderType) {
	return rendering::LoadShader(shader, rendering::ShaderType(shaderType));
}

void Rendering_DestroyShader(uint64_t shaderId) {
	rendering::UnloadShader(shaderId);
}
