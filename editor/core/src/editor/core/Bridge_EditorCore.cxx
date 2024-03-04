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

