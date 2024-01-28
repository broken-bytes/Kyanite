#include "app/AppStartScreen.hxx"
#include "app/RecentProjectsView.hxx"

#include <qboxlayout.h>
#include <qwidget.h>
#include <qlistwidget.h>
#include <qstackedlayout.h>

#include <memory>

namespace kyanite::editor {
	AppStartScreen::AppStartScreen(QWidget* parent, std::unique_ptr<AppStartScreenViewModel> viewModel) : QWidget(parent) {
		_viewModel = std::move(viewModel);

		auto hLayout = new QHBoxLayout();
		auto vLayout = new QVBoxLayout();
		hLayout->addLayout(vLayout);
		setLayout(hLayout);

		setWindowTitle("Kyanite");
		setGeometry(100, 100, 800, 600);
		setFixedSize(800, 600);

		auto recentProjectsList = _viewModel->LoadRecentProjects();

		auto recentProjectsView = new RecentProjectsView(this, recentProjectsList);
	}

	AppStartScreen::~AppStartScreen() {

	}
}