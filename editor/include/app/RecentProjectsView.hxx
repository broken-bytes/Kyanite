#pragma once


#include "project/Project.hxx"

#include <qwidget.h>

#include <vector>

namespace kyanite::editor {
	class RecentProjectsView : public QWidget {
		Q_OBJECT

	public:
		RecentProjectsView(QWidget* parent, std::vector<Project> projects);
		~RecentProjectsView();
	};
}