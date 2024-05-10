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

bool ImGui_BeginPopupContextWindow() {
	return kyanite::engine::imgui::BeginPopupContextWindow();
}

void ImGui_EndPopup() {
	kyanite::engine::imgui::EndPopup();
}

bool ImGui_BeginMenu(const char* label) {
	return kyanite::engine::imgui::BeginMenu(label);
}

void ImGui_EndMenu() {
	kyanite::engine::imgui::EndMenu();
}

bool ImGui_MenuItem(const char* label) {
	return kyanite::engine::imgui::MenuItem(label);
}

void ImGui_GetCursorPos(float* y) {
	kyanite::engine::imgui::GetCursorPos(y);
}

void ImGui_SetCursorPos(float y) {
	kyanite::engine::imgui::SetCursorPos(y);
}

void ImGui_SameLine() {
	kyanite::engine::imgui::SameLine();
}

void ImGui_Dummy(float width, float height) {
	kyanite::engine::imgui::Dummy(width, height);
}

bool ImGui_Button(const char* label, float* width, float* height) {
	return kyanite::engine::imgui::Button(label, width, height);
}

void ImGui_Label(const char* label) {
	kyanite::engine::imgui::Label(label);
}

void ImGui_Icon(const char* icon) {
	kyanite::engine::imgui::Icon(icon);
}

float ImGui_GetAvailableWidth() {
	return kyanite::engine::imgui::GetAvailableWidth();
}

bool ImGui_BeginTable(const char* name, int32_t count, int32_t flags) {
	return kyanite::engine::imgui::BeginTable(name, count, flags);
}

void ImGui_EndTable() {
	kyanite::engine::imgui::EndTable();
}

void ImGui_SetupColumn(const char* label) {
	kyanite::engine::imgui::SetupColumn(label);
}


void ImGui_TableNextRow() {
	kyanite::engine::imgui::TableNextRow();
}


void ImGui_TableSetColumnIndex(int32_t index) {
	kyanite::engine::imgui::TableSetColumnIndex(index);
}

void ImGui_Columns(int count) {
	kyanite::engine::imgui::Columns(count);
}

void ImGui_NextColumn() {
	kyanite::engine::imgui::NextColumn();
}

void ImGui_Separator() {
	kyanite::engine::imgui::Separator();
}

bool ImGui_TextField(const char* label, char* buffer, int bufferSize) {
	return kyanite::engine::imgui::TextField(label, buffer, bufferSize);
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

bool ImGui_TreeNode(const char* label) {
	return kyanite::engine::imgui::TreeNode(label);
}

void ImGui_TreePop() {
	kyanite::engine::imgui::EndTreeNode();
}

NativePointer ImGui_CreateImGuiContext() {
	return kyanite::engine::imgui::CreateImGuiContext();
}

NativePointer ImGui_GetImGuiContext() {
	return kyanite::engine::imgui::GetImGuiContext();
}