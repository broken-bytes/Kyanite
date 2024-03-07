#include "editor/core/Bridge_EditorCore.h"
#include "editor/core/EditorCore.hxx"

#include <imgui.h>

void EditorCore_Init(NativePointer imGuiContext) {
	kyanite::editor::core::Init(reinterpret_cast<ImGuiContext*>(imGuiContext));
}

void EditorCore_DrawDefaultDocking() {
	kyanite::editor::core::DrawDefaultImGuiDocking();
}

void EditorCore_BeginWindow(
	const char* title, 
	int flags, 
	int64_t id,
	void (*callback)(int64_t)
) {
	kyanite::editor::core::BeginWindow(title, flags, id, callback);
}

void EditorCore_EndWindow() {
	kyanite::editor::core::EndWindow();
}

bool EditorCore_Button(const char* label) {
	return kyanite::editor::core::Button(label);
}

void EditorCore_Label(const char* label) {
	kyanite::editor::core::Label(label);
}

void EditorCore_Separator() {
	kyanite::editor::core::Separator();
}

void EditorCore_TextField(const char* label, char* buffer, int bufferSize) {
	kyanite::editor::core::TextField(label, buffer, bufferSize);
}

void EditorCore_FloatField(const char* label, float* value) {
	kyanite::editor::core::FloatField(label, value);
}

void EditorCore_Float2Field(const char* label, float* value) {
	kyanite::editor::core::Float2Field(label, value);
}

void EditorCore_Float3Field(const char* label, float* value) {
	kyanite::editor::core::Float3Field(label, value);
}

void EditorCore_Float4Field(const char* label, float* value) {
	kyanite::editor::core::Float4Field(label, value);
}

void EditorCore_IntField(const char* label, int* value) {
	kyanite::editor::core::IntField(label, value);
}

void EditorCore_Checkbox(const char* label, bool* value) {
	kyanite::editor::core::Checkbox(label, value);
}

void EditorCore_BeginHorizontal() {
	kyanite::editor::core::BeginHorizontal();
}

void EditorCore_EndHorizontal() {
	kyanite::editor::core::EndHorizontal();
}

