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
	EXPORTED void ImGui_Init(NativePointer imGuiContext);

	/**
	* @brief Draw the default docking
	*/
	EXPORTED void ImGui_DrawDefaultDocking();

	/**
	* @brief Begin a window
	* @param title The title of the window
	* @param p_open A pointer to a boolean that will be set to true if the window is open
	* @param flags The flags to use for the window
	*/
	EXPORTED void ImGui_BeginWindow(
		const char* title,
		int flags,
		int64_t id,
		void (*callback)(int64_t)
	);

	/**
	* @brief End a window
	*/
	EXPORTED void ImGui_EndWindow();

	/**
	* @brief Begin a menu window
	* @param title The title of the window
	* @param the mouse button to open the menu
	*/
	EXPORTED bool ImGui_BeginPopupContextWindow();

	/**
	* @brief End a menu window
	*/
	EXPORTED void ImGui_EndPopup();

	/**
	* @brief Begin a menu
	* @param label The label of the menu
	*/
	EXPORTED bool ImGui_BeginMenu(const char* label);

	/**
	* @brief End a menu
	*/
	EXPORTED void ImGui_EndMenu();

	/**
	* @brief Begin a menu item
	*/
	EXPORTED bool ImGui_MenuItem(const char* label);

	/**
	* @brief Gets the cursor position
	* @param y A pointer to the y position
	*/
	EXPORTED void ImGui_GetCursorPos(float* y);

	/**
	* @brief Set the cursor position to the given y
	* @param y The y position to set the cursor to
	*/
	EXPORTED void ImGui_SetCursorPos(float y);

	/**
	* @brief Makes the same line
	*/
	EXPORTED void ImGui_SameLine();

	/**
	* @brief Draw a dummy
	* @param width The width of the dummy
	* @param height The height of the dummy
	*/
	EXPORTED void ImGui_Dummy(float width, float height);

	/**
	* @brief Draw a button
	* @param label The label of the button
	* @param width The width of the button
	* @param height The height of the button
	* @return True if the button was clicked
	*/
	EXPORTED bool ImGui_Button(const char* label, float* width, float* height);

	/**
	* @brief Draw a label
	* @param label The label to draw
	*/
	EXPORTED void ImGui_Label(const char* label);

	/**
	* @brief Draw an icon
	* @param icon The icon to draw
	*/
	EXPORTED void ImGui_Icon(const char* icon);

	/**
	* @brief Gets the available width
	* @return The available width
	*/
	EXPORTED float ImGui_GetAvailableWidth();

	/**
	* @brief Creates a table
	* @param name The name of the table
	* @param count The number of columns
	* @param flags The flags to use
	*/
	EXPORTED bool ImGui_BeginTable(const char* name, int32_t count, int32_t flags);

	/**
	* @brief Ends a table
	*/
	EXPORTED void ImGui_EndTable();

	/**
	* @brief Creates a table column
	*/
	EXPORTED void ImGui_SetupColumn(const char* label);

	/**
	* @brief Goes to the next row
	*/
	EXPORTED void ImGui_TableNextRow();

	/**
	* @brief Sets the column index
	* @param index The index of the column
	*/
	EXPORTED void ImGui_TableSetColumnIndex(int32_t index);

	/**
	* @brief Sets the next column
	*/
	EXPORTED void ImGui_NextColumn();

	/**
	* @brief Draws a separator
	*/
	EXPORTED void ImGui_Separator();

	/**
	* @brief Draw a text field
	* @param label The label of the text field
	* @param buffer The buffer to store the text in
	* @param bufferSize The size of the buffer
	*/
	EXPORTED bool ImGui_TextField(const char* label, char* buffer, int bufferSize);

	/**
	* @brief Draw a float field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void ImGui_FloatField(const char* label, float* value);

	/**
	* @brief Draw a float2 field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void ImGui_Float2Field(const char* label, float* value);

	/**
	* @brief Draw a float3 field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void ImGui_Float3Field(const char* label, float* value);

	/**
	* @brief Draw a float4 field
	* @param label The label of the float field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void ImGui_Float4Field(const char* label, float* value);

	/**
	* @brief Draw a int field
	* @param label The label of the int field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void ImGui_IntField(const char* label, int* value);

	/**
	* @brief Draw a checkbox
	* @param label The label of the checkbox field
	* @param value A pointer to the value to edit
	*/
	EXPORTED void ImGui_Checkbox(const char* label, bool* value);

	/**
	* @brief Begin a horizontal layout
	*/
	EXPORTED void ImGui_BeginHorizontal();

	/**
	* @brief Ends a horizontal layout
	*/
	EXPORTED void ImGui_EndHorizontal();

	/**
	* @brief Shows a tree node
	* @param label The label of the tree node
	* @return True if the tree node is open
	*/
	EXPORTED bool ImGui_TreeNode(const char* label);

	/**
	* @brief Ends a tree node
	*/
	EXPORTED void ImGui_TreePop();

	/**
	 * @brief Creates an ImGui context
	 * @return NativePointer Returns a pointer to the context
	 */
	EXPORTED NativePointer ImGui_CreateImGuiContext();

	/**
	 * @brief Getst he current ImGui context
	 * @return NativePointer Returns a pointer to the context
	 */
	EXPORTED NativePointer ImGui_GetImGuiContext();
#ifdef __cplusplus
}
#endif
