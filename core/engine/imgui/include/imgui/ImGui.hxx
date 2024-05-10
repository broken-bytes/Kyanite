#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <imgui.h>

#include <functional>
#include <string>

namespace kyanite::engine::imgui {
	/**
	* @brief Initializes the editor core module
	*/
	EXPORTED auto Init(ImGuiContext* context) -> void;

	/**
	* @brief Begin a window
	* @param title The title of the window
	* @param flags The flags to use
	* @param id The id of the window. Needed for the callback
	* @param callback The callback to call when the window is closed
	*/
	EXPORTED auto BeginWindow(std::string title, int flags, int64_t id, void(*callback)(int64_t)) -> void;

	/**
	* @brief End a window
	*/
	EXPORTED auto EndWindow() -> void;

	/**
	* @brief Begin a menu window
	* @param title The title of the window
	* @param the mouse button to open the menu
	*/
	EXPORTED bool BeginPopupContextWindow();

	/**
	* @brief End a menu window
	*/
	EXPORTED void EndPopup();

	/**
	* @brief Begin a menu
	* @param label The label of the menu
	*/
	EXPORTED bool BeginMenu(const char* label);

	/**
	* @brief End a menu
	*/
	EXPORTED void EndMenu();

	/**
	* @brief Begin a menu item
	*/
	EXPORTED bool MenuItem(const char* label);

	/**
	* @brief Gets the cursor position
	* @param y The y position of the cursor
	*/
	EXPORTED void GetCursorPos(float* y);

	/**
	* @brief Set the cursor position to the given y
	* @param y The y position to set the cursor to
	*/
	EXPORTED void SetCursorPos(float y);

	/**
	* @brief Makes the same line
	*/
	EXPORTED void SameLine();

	/**
	* @brief Draw a dummy
	* @param width The width of the dummy
	* @param height The height of the dummy
	*/
	EXPORTED void Dummy(float width, float height);

	/**
	* @brief Draws a reference selector
	* @param window The window to draw the selector in
	* @param label The label to display
	* @param onReferenceSet The callback to call when a reference is set
	*/
	EXPORTED auto DrawReferenceSelector(NativePointer window, std::string label, std::function<void(std::string)> onReferenceSet) -> void;

	/**
	* @brief Draws a button
	* @param window The window to draw the button in
	* @param label The label to display
	* @param width The width of the button
	* @param height The height of the button
	*/
	EXPORTED auto Button(std::string_view label, float* width, float* height) -> bool;

	/**
	* @brief Draws a label
	* @param window The window to draw the label in
	* @param label The label to display
	*/
	EXPORTED auto Label(std::string label) -> void;

	/**
	* @brief Creates a table
	* @param name The name of the table
	* @param count The number of columns
	* @param flags The flags to use
	*/
	bool BeginTable(const char* name, int32_t count, int32_t flags);

	/**
	* @brief Ends a table
	*/
	void EndTable();

	/**
	* @brief Creates a table column
	*/
	void SetupColumn(const char* label);

	/**
	* @brief Goes to the next row
	*/
	void TableNextRow();

	/**
	* @brief Sets the column index
	* @param index The index of the column
	*/
	void TableSetColumnIndex(int32_t index);

	/**
	* @brief Sets a number of columns
	* @param count The number of columns
	*/
	EXPORTED void Columns(int count);

	/**
	* @brief Sets the next column
	*/
	EXPORTED void NextColumn();

	/**
	* @brief Draws an icon
	* @param icon The icon to display
	*/
	EXPORTED auto Icon(std::string icon) -> void;

	/**
	* @brief Gets the available width
	* @return The available width
	*/
	EXPORTED float GetAvailableWidth();

	/**
	* @brief Draws a separator
	*/
	EXPORTED auto Separator() -> void;

	/**
	* @brief Draws a text field
	* @param label The label to display
	* @param buffer The buffer to store the text in
	* @param bufferSize The size of the buffer
	*/
	EXPORTED auto TextField(const char* label, char* buffer, int bufferSize) -> bool;

	/**
	* @brief Draws a float field
	* @param label The label to display
	* @param value The value to store the float in
	*/
	EXPORTED auto FloatField(const char* label, float* value) -> void;

	/**
	* @brief Draws a float2 field
	* @param label The label to display
	* @param value The value to store the float2 in
	*/
	EXPORTED auto Float2Field(const char* label, float* value) -> void;

	/**
	* @brief Draws a float3 field
	* @param label The label to display
	* @param value The value to store the float3 in
	*/
	EXPORTED auto Float3Field(const char* label, float* value) -> void;

	/**
	* @brief Draws a float4 field
	* @param label The label to display
	* @param value The value to store the float4 in
	*/
	EXPORTED auto Float4Field(const char* label, float* value) -> void;

	/**
	* @brief Draws an int field
	* @param label The label to display
	* @param value The value to store the int in
	*/
	EXPORTED auto IntField(const char* label, int* value) -> void;

	/**
	* @brief Draws a checkbox
	* @param label The label to display
	* @param value The value to store the bool in
	*/
	EXPORTED auto Checkbox(const char* label, bool* value) -> void;

	/**
	* @brief Begins a horizontal layout
	*/
	EXPORTED auto BeginHorizontal() -> void;

	/**
	* @brief Ends a horizontal layout
	*/
	EXPORTED auto EndHorizontal() -> void;

	/**
	* @brief Clears the window
	* @param window The window to clear
	*/
	EXPORTED auto Clear(NativePointer window) -> void;

	/**
	* @brief Draws a tree node
	* @param label The label to display
	* @return Whether the node is open
	*/
	EXPORTED auto TreeNode(const char* label) -> bool;

	/**
	* @brief Ends a tree node
	*/
	EXPORTED auto EndTreeNode() -> void;

	/**
	* @brief Draws the editor default docking
	*/
	EXPORTED auto DrawDefaultImGuiDocking() -> void;

	/**
	* @brief Creates an ImGui context
	* @return The created ImGui context
	*/
	EXPORTED auto CreateImGuiContext() -> ImGuiContext*;

	/**
	* @brief Gets the ImGui context
	* @return The ImGui context
	*/
	EXPORTED auto GetImGuiContext() -> ImGuiContext*;
}