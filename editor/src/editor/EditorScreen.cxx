#include "editor/EditorScreen.hxx"

#include <filesystem>
#include <sstream>

namespace kyanite::editor {
	EditorScreen::EditorScreen(
		QWidget* parent, 
		std::string projectPath, 
		bool createNewProject,
		std::string projectName,
		std::unique_ptr<EditorScreenViewModel> viewModel
	) : _projectPath(projectPath), _viewModel(std::move(viewModel)) {

		Project project = {};
		if(createNewProject) {
			project = _viewModel->CreateProject(_projectPath, projectName);
		}
		else {
			project = _viewModel->LoadProject(projectPath);
		}

		std::stringstream ss;
		ss << "Kyanite Editor - " << project.name;
		setWindowTitle(ss.str().c_str());
	}

	EditorScreen::~EditorScreen() {

	}
}