#include "app/App.hxx"
#include "app/AppStartScreen.hxx"
#include "app/AppStartScreenViewModel.hxx"
#include <assetpackages/AssetPackages.hxx>
#include <rendering/Texture.hxx>

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmainwindow.h>

#include <filesystem>
#include <memory>
#include <sstream>

namespace assetpackages = kyanite::engine::assetpackages;
namespace editor = kyanite::editor;

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	app.setApplicationName("Kyanite");
	app.setApplicationVersion("0.0.1");	

	auto startScreen = new kyanite::editor::AppStartScreen(nullptr, std::make_unique<editor::AppStartScreenViewModel>());

	// Show the main window
	startScreen->show();

	auto currentDir = std::filesystem::current_path();

	auto packageDir = currentDir / "packages";

	auto testPackage = packageDir / "test.kpack";

	auto texture = kyanite::engine::rendering::Texture(0, 128, 128, 4);

	auto package = assetpackages::CreateAssetPackage(testPackage.string());
	assetpackages::AddToAssetPackage(package, "test", texture);

	auto data = assetpackages::LoadAssetFromPackage<kyanite::engine::rendering::Texture>(package, "test");
	app.exec();
}