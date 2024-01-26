#pragma once

#include "project/Project.hxx"
#include "project/ProjectService.hxx"
#include "editor/FileWatchdog.hxx"
#include "mappers/ModelMapper.hxx"
#include <assets/AssetDatabase.hxx>

#include <qwidget.h>

#include <memory>
#include <string>
#include <vector>

namespace kyanite::editor {
	class EditorScreenViewModel {
	public:
		EditorScreenViewModel(
			std::unique_ptr<ProjectService> service,
			std::unique_ptr<AssetDatabase> assetDatabase
		);
		~EditorScreenViewModel();

		auto LoadProject(const std::string& path) -> Project;
		auto CreateProject(const std::string& path, std::string& name) -> Project;
		auto InitializeEngine(WId window) -> void;
		auto SetupEditorEnvironment() -> void;

	private:
		std::unique_ptr<ProjectService> _service;
		std::unique_ptr<AssetDatabase> _assetDatabase;
		std::unique_ptr<FileWatchdog> _sourceWatchdog;
		std::unique_ptr<FileWatchdog> _contentWatchdog;
		std::unique_ptr<mappers::ModelMapper> _modelMapper;

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
	};
}