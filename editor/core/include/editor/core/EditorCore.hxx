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
	* @param open A pointer to a boolean that indicates if the window is open
	* @param flags The flags to use
	*/
	EXPORTED auto BeginWindow(std::string title, bool* open, int flags) -> void;

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
	EXPORTED auto DrawButton(NativePointer window, std::string label, std::function<void()> action) -> void;

	/**
	* @brief Draws a label
	* @param window The window to draw the label in
	* @param label The label to display
	*/
	EXPORTED auto DrawLabel(NativePointer window, std::string label) -> void;

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