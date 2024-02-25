#include "rendering/Bridge_Rendering.h"
#include "rendering/Rendering.hxx"
#include "rendering/Shader.hxx"

#include <imgui.h>

namespace rendering = kyanite::engine::rendering;

void Rendering_Init(NativePointer window) {
	rendering::Init(window);
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

// ImGUI functions
void Rendering_StartWindow(const char* name, bool* p_open) {
	ImGui::Begin(name, p_open);
}

void Rendering_EndWindow() {
	ImGui::End();
}

void Rendering_DrawText(const char* text) {
	ImGui::Text(text);
}

bool Rendering_DrawButton(const char* label) {
	return ImGui::Button(label);
}

float Rendering_DrawSlider(const char* label, float value, float min, float max) {
	ImGui::SliderFloat(label, &value, min, max);
	return value;
}

void Rendering_DrawColorPicker(const char* label, float* color) {
	ImGui::ColorEdit3(label, color);
}

void Rendering_DrawVector2(const char* label, float* values) {
	ImGui::InputFloat2(label, values, "%.2", ImGuiInputTextFlags_EnterReturnsTrue);
}

void Rendering_DrawVector3(const char* label, float* values) {
	ImGui::InputFloat3(label, values, "%.2", ImGuiInputTextFlags_EnterReturnsTrue);
}

void Rendering_DrawVector4(const char* label, float* values) {
	ImGui::InputFloat4(label, values, "%.2", ImGuiInputTextFlags_EnterReturnsTrue);
}

int Rendering_DrawComboBox(const char* label, int currentItem, const char** items, int itemsCount) {
	ImGui::Combo(label, &currentItem, items, itemsCount);
	return currentItem;
}

void Rendering_DrawImage(uint64_t textureId, uint32_t width, uint32_t height) {
	ImGui::Image((void*)(intptr_t)textureId, ImVec2(width, height));
}

void Rendering_DrawDragAndDropTarget(const char* label, void* target) {
	ImGui::Text("Drag and drop a texture here");
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
