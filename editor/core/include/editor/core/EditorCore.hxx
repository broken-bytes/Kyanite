#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <functional>
#include <string>
namespace kyanite::editor::core {
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
}