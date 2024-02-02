#pragma once

#include "editor/core/project/Project.hxx"
#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <filesystem>
#include <functional>
#include <string>

namespace kyanite::editor::core {
	/**
	* @brief Saves the metadata to a file
	* @param path The path to save the metadata to
	* @param metadata The metadata to save
	*/
	EXPORTED auto SaveMetadata(std::string path, std::string metadata) -> void;

	template<typename T>
	EXPORTED auto LoadMetadata(std::string path) -> T;

	auto LoadProject(const std::string& path) -> Project;
	auto CreateProject(const std::string& path, std::string& name) -> Project;
	auto InitializeEngine() -> void;
	auto SetupEditorEnvironment() -> void;

	template<typename T>
	auto SaveMeta(std::string name, std::filesystem::path path, T& meta) -> void;

	template<typename T>
	auto LoadMeta(std::filesystem::path path) -> T;

	template<typename T>
	auto SaveBlob(std::string uuid, T& blob) -> void;

	auto SaveModelMeta(std::string name, std::filesystem::directory_entry path) -> std::string;
	auto SaveModelData(std::string uuid, std::vector<uint8_t>& data) -> void;

	auto CompareLastAssetDatabase() -> void;
	auto HandleFileAdded(std::filesystem::directory_entry file) -> void;
	auto HandleFileModified(std::filesystem::directory_entry file) -> void;
	auto HandleFileRemoved(std::filesystem::path path) -> void;

	auto HandleMetaData(std::filesystem::directory_entry file) -> void;
	auto HandleData(std::filesystem::directory_entry file) -> void;

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