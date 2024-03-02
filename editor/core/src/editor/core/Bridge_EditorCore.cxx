#include "editor/core/Bridge_EditorCore.h"
#include "editor/core/EditorCore.hxx"

#include <imgui.h>

void EditorCore_Init(NativePointer imGuiContext) {
	kyanite::editor::core::Init(reinterpret_cast<ImGuiContext*>(imGuiContext));
}

void EditorCore_DrawDefaultDocking() {
	kyanite::editor::core::DrawDefaultImGuiDocking();
}

void EditorCore_BeginWindow(const char* title, bool* p_open, int flags) {
	kyanite::editor::core::BeginWindow(title, p_open, flags);
}

void EditorCore_EndWindow() {
	kyanite::editor::core::EndWindow();
}

