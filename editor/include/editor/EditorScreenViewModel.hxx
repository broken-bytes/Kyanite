#pragma once

#include "project/Project.hxx"
#include "project/ProjectService.hxx"

#include <memory>
#include <string>

namespace kyanite::editor {
	class EditorScreenViewModel {
	public:
		EditorScreenViewModel(std::unique_ptr<ProjectService> service);
		~EditorScreenViewModel();

		auto LoadProject(const std::string& path) -> Project;
		auto CreateProject(const std::string& path, std::string& name) -> Project;

	private:
		std::unique_ptr<ProjectService> _service;
	};
}