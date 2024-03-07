#pragma once

#include "editor/core/project/Project.hxx"
#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <imgui.h>

#include <filesystem>
#include <functional>
#include <string>

namespace kyanite::editor::core {
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
	* @param action The callback to call when the button is pressed
	*/
	EXPORTED auto Button(std::string_view label) -> bool;

	/**
	* @brief Draws a label
	* @param window The window to draw the label in
	* @param label The label to display
	*/
	EXPORTED auto Label(std::string label) -> void;

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
	EXPORTED auto TextField(const char* label, char* buffer, int bufferSize) -> void;

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
	* @brief Draws the editor default docking
	*/
	EXPORTED auto DrawDefaultImGuiDocking() -> void;
}