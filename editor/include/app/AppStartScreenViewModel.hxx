#pragma once

#include "project/Project.hxx"

#include <vector>

namespace kyanite::editor {

	class AppStartScreenViewModel {
		public:
		AppStartScreenViewModel();
		~AppStartScreenViewModel();

		auto CreateProject() -> void;
		auto LoadRecentProjects() -> std::vector<Project>;
	};
}