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
	EXPORTED void EditorCore_BeginWindow(
		const char* title, 
		int flags, 
		int64_t id, 
		void (*callback)(int64_t)
	);

	/**
	* @brief End a window
	*/
	EXPORTED void EditorCore_EndWindow();

	/**
	* @brief Draw a button
	* @param label The label of the button
	* @return True if the button was clicked
	*/
	EXPORTED bool EditorCore_Button(const char* label);

	/**
	* @brief Draw a label
	* @param label The label to draw
	*/
	EXPORTED void EditorCore_Label(const char* label);
#ifdef __cplusplus
}
#endif
