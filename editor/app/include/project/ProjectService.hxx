#pragma once

#include "Project.hxx"

#include <filesystem>
#include <string>

namespace kyanite::editor {
	class ProjectService {
	public:
		ProjectService();
		~ProjectService();

		auto LoadProject(const std::string& path) -> Project;
		auto CreateProject(const std::string& path, std::string& name) -> Project;

		auto AssembliesPath() -> std::filesystem::path;
		auto BlobsPath() -> std::filesystem::path;
		auto BuildPath() -> std::filesystem::path;
		auto CachePath() -> std::filesystem::path;
		auto ContentPath() -> std::filesystem::path;
		auto LogsPath() -> std::filesystem::path;
		auto ProjectPath() -> std::filesystem::path;
		auto SourcePath() -> std::filesystem::path;

	private:
		std::filesystem::path _projectPath;
		auto CreateFolderStructure(const std::string& path, const std::string& name) -> void;
		auto CreateProjectFile(const std::string& path, const std::string& name) -> void;
		auto CreateAssetDatabase(const std::string& path, const std::string& name) -> void;
	};
}