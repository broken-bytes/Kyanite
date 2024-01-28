#pragma once

#include <qwidget.h>

namespace kyanite::editor {
	class WorldView: public QWidget {
	public:
		WorldView(QWidget* parent);
		~WorldView();
	};
}