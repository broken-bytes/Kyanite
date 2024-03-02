#include "app/App.hxx"
#include "app/AppStartScreen.hxx"
#include "app/AppStartScreenViewModel.hxx"
#include "editor/Editor.hxx"
#include <editor/core/project/ProjectService.hxx>
#include <assetpackages/AssetPackages.hxx>
#include <logger/Logger.hxx>
#include <logger/ConsoleLogger.hxx>

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qmessagebox.h>

#include <filesystem>
#include <memory>
#include <sstream>

#pragma comment(lib, "KyaniteEngine.lib")
#pragma comment(lib, "KyaniteEditor.lib")

namespace assetpackages = kyanite::engine::assetpackages;
namespace editor = kyanite::editor;

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	app.setApplicationName("Kyanite");
	app.setApplicationVersion("0.0.1");

	// Iterate the command line arguments and check for '-p' or '--project'. If found, the next argument is the project file to open.
	// Also check for -c or --create. If found, create a new project file and open it.
	// If both -p and -c are found, -p takes precedence.
	// -c also needs a name for the project which is passed via the next argument.

	std::string projectFile = "";
	std::string name = "";
	bool createProject = false;

	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-p" || arg == "--project") {
			if (i + 1 < argc) {
				projectFile = argv[i + 1];
				createProject = false;
				break;
			}
		}
		else if (arg == "-c" || arg == "--create") {
			if (i + 1 < argc) {
				projectFile = argv[i + 1];
				if (i + 2 >= argc) {
					QMessageBox::critical(nullptr, "Error", "The project name is missing.");
					exit(-1);
				}
				name = argv[i + 2];
				createProject = true;
				break;
			}
		}
	}

	// Set the logger
	auto logger = std::make_shared<kyanite::engine::logging::ConsoleLogger>();
	kyanite::engine::logging::logger::SetLogger(std::move(logger));


	// Check if the project file exists. If it does, open it. If not, open the start screen.

	if (projectFile != "") {
		auto editor = new kyanite::editor::Editor(
			projectFile,
			createProject,
			name,
			std::make_unique<kyanite::editor::ProjectService>(),
			std::make_unique<kyanite::editor::AssetDatabase>()
		);
	}
	else {
		auto startScreen = new kyanite::editor::AppStartScreen(nullptr, std::make_unique<editor::AppStartScreenViewModel>());
		startScreen->show();
	}

	app.exec();
}