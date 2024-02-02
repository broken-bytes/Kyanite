#pragma once

#include "editor/FileWatchdog.hxx"
#include <editor/core/assets/AssetDatabase.hxx>
#include <editor/core/project/ProjectService.hxx>
#include <editor/core/mappers/ModelMapper.hxx>

#include <qobject.h>
#include <qprogressdialog.h>

#include <memory>
#include <string>
#include <vector>

namespace kyanite::editor {
	class Editor: public QObject {
		Q_OBJECT
	public:
		Editor(
			std::string projectPath, 
			bool createNewProject, 
			std::string projectName,
			std::unique_ptr<ProjectService> service,
			std::unique_ptr<AssetDatabase> assetDatabase
		);
		~Editor();

		auto LoadProject(const std::string& path) -> Project;
		auto CreateProject(const std::string& path, std::string& name) -> Project;
		auto InitializeEngine() -> void;
		auto SetupEditorEnvironment() -> void;

	public slots:
		auto OnFileAdded(std::filesystem::directory_entry file) -> void;
		auto OnFileModified(std::filesystem::directory_entry file) -> void;
		auto OnFileRemoved(std::filesystem::path path) -> void;

	private:
		std::unique_ptr<ProjectService> _service;
		std::unique_ptr<AssetDatabase> _assetDatabase;
		std::unique_ptr<FileWatchdog> _sourceWatchdog;
		std::unique_ptr<FileWatchdog> _contentWatchdog;
		std::unique_ptr<mappers::ModelMapper> _modelMapper;
		std::string _projectPath;

		auto SetupEngine() -> void;

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
		auto ShowAssetProcessingDialog() -> QProgressDialog*;
	};
}