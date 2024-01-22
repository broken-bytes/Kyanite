#pragma once

#include "project/Project.hxx"
#include "project/ProjectService.hxx"
#include "assets/AssetDatabase.hxx"

#include <qwidget.h>

#include <memory>
#include <string>

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

	private:
		std::unique_ptr<ProjectService> _service;
		std::unique_ptr<AssetDatabase> _assetDatabase;
	};
}