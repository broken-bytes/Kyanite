#include "imgui/Bridge_ImGui.h"
#include "imgui/ImGui.hxx"

#include <imgui.h>

void ImGui_Init(NativePointer imGuiContext) {
	kyanite::engine::imgui::Init(reinterpret_cast<ImGuiContext*>(imGuiContext));
}

void ImGui_DrawDefaultDocking() {
	kyanite::engine::imgui::DrawDefaultImGuiDocking();
}

void ImGui_BeginWindow(
	const char* title, 
	int flags, 
	int64_t id,
	void (*callback)(int64_t)
) {
	kyanite::engine::imgui::BeginWindow(title, flags, id, callback);
}

void ImGui_EndWindow() {
	kyanite::engine::imgui::EndWindow();
}

bool ImGui_Button(const char* label) {
	return kyanite::engine::imgui::Button(label);
}

void ImGui_Label(const char* label) {
	kyanite::engine::imgui::Label(label);
}

void ImGui_Separator() {
	kyanite::engine::imgui::Separator();
}

void ImGui_TextField(const char* label, char* buffer, int bufferSize) {
	kyanite::engine::imgui::TextField(label, buffer, bufferSize);
}

void ImGui_FloatField(const char* label, float* value) {
	kyanite::engine::imgui::FloatField(label, value);
}

void ImGui_Float2Field(const char* label, float* value) {
	kyanite::engine::imgui::Float2Field(label, value);
}

void ImGui_Float3Field(const char* label, float* value) {
	kyanite::engine::imgui::Float3Field(label, value);
}

void ImGui_Float4Field(const char* label, float* value) {
	kyanite::engine::imgui::Float4Field(label, value);
}

void ImGui_IntField(const char* label, int* value) {
	kyanite::engine::imgui::IntField(label, value);
}

void ImGui_Checkbox(const char* label, bool* value) {
	kyanite::engine::imgui::Checkbox(label, value);
}

void ImGui_BeginHorizontal() {
	kyanite::engine::imgui::BeginHorizontal();
}

void ImGui_EndHorizontal() {
	kyanite::engine::imgui::EndHorizontal();
}


NativePointer ImGui_CreateImGuiContext() {
	return kyanite::engine::imgui::CreateImGuiContext();
}

NativePointer ImGui_GetImGuiContext() {
	return kyanite::engine::imgui::GetImGuiContext();
}