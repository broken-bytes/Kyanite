#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#ifdef __cplusplus
extern "C" {
#endif
	/**
	* @brief Initialize the editor core
	* @param imGuiContext The ImGui context to use
	*/
	EXPORTED void EditorCore_Init(NativePointer imGuiContext);

	/**
	* @brief Draw the default docking
	*/
	EXPORTED void EditorCore_DrawDefaultDocking();

	/**
	* @brief Begin a window
	* @param title The title of the window
	* @param p_open A pointer to a boolean that will be set to true if the window is open
	* @param flags The flags to use for the window
	*/
	EXPORTED void EditorCore_BeginWindow(const char* title, bool* p_open, int flags);

	/**
	* @brief End a window
	*/
	EXPORTED void EditorCore_EndWindow();
#ifdef __cplusplus
}
#endif
