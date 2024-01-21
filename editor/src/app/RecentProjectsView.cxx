#include "app/RecentProjectsView.hxx"

#include <qboxlayout>
#include <qlabel>
#include <qlistwidget.h>
#include <qstring.h>

namespace kyanite::editor {
	RecentProjectsView::RecentProjectsView(QWidget* parent, std::vector<Project> projects) : QWidget(parent) {
		auto box = new QVBoxLayout(this);
		box->setContentsMargins(16, 16, 16, 16);
		box->setSizeConstraint(QLayout::SetMinAndMaxSize);

		auto* label = new QLabel("Recent Projects");
		label->setFont(QFont("Arial", 18, QFont::Normal));

		box->addWidget(label);

		if (!projects.empty()) {
			auto* list = new QListWidget();

			for (auto& project : projects) {
				auto item = new QListWidgetItem(project.name.c_str(), list);
				item->setToolTip(QString(project.name.c_str()));
			}

			box->addWidget(list);
		}

		this->setLayout(box);
	}

	RecentProjectsView::~RecentProjectsView() {

	}
}