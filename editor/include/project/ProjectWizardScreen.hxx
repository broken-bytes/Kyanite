#pragma once

#include <qwidget.h>

namespace kyanite::editor {
	class ProjectWizardScreen: public QWidget {
	public:
		ProjectWizardScreen(QWidget* parent);
		~ProjectWizardScreen();
	};
}