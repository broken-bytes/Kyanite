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

	/**
	* @brief Draws a separator
	*/
	EXPORTED void EditorCore_Separator();

	/**
	* @brief Draw a text field
	* @param label The label of the text field
	* @param buffer The buffer to store the text in
	* @param bufferSize The size of the buffer
	*/
	EXPORTED void EditorCore_TextField(const char* label, char* buffer, int bufferSize);

	/**
	* @brief Draw a float field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void EditorCore_FloatField(const char* label, float* value);

	/**
	* @brief Draw a float2 field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void EditorCore_Float2Field(const char* label, float* value);

	/**
	* @brief Draw a float3 field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void EditorCore_Float3Field(const char* label, float* value);

	/**
	* @brief Draw a float4 field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void EditorCore_Float4Field(const char* label, float* value);

	/**
	* @brief Draw a int field
	* @param label The label of the int field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void EditorCore_IntField(const char* label, int* value);

	/**
	* @brief Draw a checkbox
	* @param label The label of the checkbox field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void EditorCore_Checkbox(const char* label, bool* value);

	/**
	* @brief Begin a horizontal layout
	*/
	EXPORTED void EditorCore_BeginHorizontal();

	/**
	* @brief Ends a horizontal layout
	*/
	EXPORTED void EditorCore_EndHorizontal();
#ifdef __cplusplus
}
#endif
