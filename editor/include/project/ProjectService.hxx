#pragma once

#include "Project.hxx"

#include <string>

namespace kyanite::editor {
	class ProjectService {
	public:
		ProjectService();
		~ProjectService();

		auto LoadProject(const std::string& path) -> Project;
		auto CreateProject(const std::string& path, std::string& name) -> Project;

	private:
		auto CreateFolderStructure(const std::string& path, const std::string& name) -> void;
		auto CreateProjectFile(const std::string& path, const std::string& name) -> void;
		auto CreateAssetDatabase(const std::string& path, const std::string& name) -> void;
	};
}