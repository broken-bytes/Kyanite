#include "editor/EditorScreen.hxx"
#include "editor/WorldView.hxx"

#include <qgridlayout.h>

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

		auto worldView = new WorldView(this);
		SetupLayout(worldView);
		SetupEngine(worldView->winId());
		_viewModel->SetupEditorEnvironment();
	}

	EditorScreen::~EditorScreen() {

	}

	auto EditorScreen::SetupLayout(QWidget* worldView) -> void {
		auto layout = new QGridLayout(this);
		setLayout(layout);

		layout->addWidget(worldView, 1, 1);
	}

	auto EditorScreen::SetupEngine(WId windowId) -> void {
		_viewModel->InitializeEngine(windowId);
	}
}