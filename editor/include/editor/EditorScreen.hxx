#pragma once

#include "EditorScreenViewModel.hxx"

#include <qwidget.h>

#include <memory>
#include <string>
#include <vector>

namespace kyanite::editor {
	class EditorScreen : public QWidget {
		Q_OBJECT
	public:
		EditorScreen(
			QWidget* parent, 
			std::string projectPath, 
			bool createNewProject, 
			std::string projectName,
			std::unique_ptr<EditorScreenViewModel> viewModel
		);
		~EditorScreen();

	private:
		std::unique_ptr<EditorScreenViewModel> _viewModel;
		std::string _projectPath;

		auto SetupLayout(QWidget* worldView) -> void;
		auto SetupEngine(WId windowId) -> void;
	};
}