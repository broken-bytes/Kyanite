#pragma once

#include "AppStartScreenViewModel.hxx"

#include <qwidget.h>

#include <memory>
#include <string>
#include <vector>

namespace kyanite::editor {
	class AppStartScreen : public QWidget {
		Q_OBJECT
	public:
		AppStartScreen(QWidget* parent, std::unique_ptr<AppStartScreenViewModel> viewModel);
		~AppStartScreen();

	private:
		std::unique_ptr<AppStartScreenViewModel> _viewModel;
	};
}