#include "editor/Editor.hxx"
#include <editor/core/meta/Meta.hxx>
#include <editor/core/meta/ModelMeta.hxx>
#include <core/Core.hxx>
#include <rendering/Material.hxx>
#include <rendering/Model.hxx>
#include <rendering/Texture.hxx>
#include <logger/Logger.hxx>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <qfuture.h>
#include <qfuturewatcher.h>
#include <qtconcurrentrun.h>

#include <filesystem>
#include <sstream>

// NOTE: These functions come from the Swift libraries. They are *not* unused 
extern "C" void kyanitemain(bool);
extern "C" void kyaniteeditormain();

std::thread engineThread;
std::thread editorThread;

namespace kyanite::editor {
	Editor::Editor(
		std::string projectPath,
		bool createNewProject,
		std::string projectName,
		std::unique_ptr<ProjectService> service,
		std::unique_ptr<AssetDatabase> assetDatabase
	) : _projectPath(projectPath), _service(std::move(service)), _assetDatabase(std::move(assetDatabase)) {

		Project project = {};
		if (createNewProject) {
			project = CreateProject(_projectPath, projectName);
		}
		else {
			project = LoadProject(projectPath);
		}

		SetupEngine();
		SetupEditorEnvironment();
	}

	Editor::~Editor() {

	}

	auto Editor::SetupEngine() -> void {
		InitializeEngine();
	}

	auto Editor::LoadProject(const std::string& path) -> Project {
		auto project = _service->LoadProject(path);

		return project;
	}

	auto Editor::CreateProject(const std::string& path, std::string& name) -> Project {
		auto project = _service->CreateProject(path, name);

		return project;
	}

	auto Editor::InitializeEngine() -> void {
		// Start the engine in a separate thread so it does not interfere with the editor
		engineThread = std::thread([this]() {
			kyanitemain(true);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		kyaniteeditormain();

		try {
			_assetDatabase->Load(_service->CachePath());
		}
		catch (std::exception& e) {
			kyanite::engine::logging::logger::Error(
				"Failed to load asset database: " + std::string(e.what())
			);

			// If the asset database fails to load, we stop execution
			// This is because the engine relies on the asset database to function
			throw std::runtime_error("Failed to start editor");
		}
	}

	template auto Editor::SaveMeta<meta::ModelMeta>(
		std::string name,
		std::filesystem::path path,
		meta::ModelMeta& meta
	) -> void;

	template<typename T>
	auto Editor::SaveMeta(std::string name, std::filesystem::path path, T& meta) -> void {
		std::stringstream ss;
		{
			cereal::JSONOutputArchive archive(ss);
			archive(CEREAL_NVP(meta));
		}

		// Move string file data to buffer
		std::vector<uint8_t> buffer;
		buffer.resize(ss.str().size());
		auto str = ss.str();
		std::memcpy(buffer.data(), str.c_str(), str.size());

		auto metaPath = path.string() + ".meta";
		kyanite::engine::core::SaveBufferToFile(metaPath, buffer);
	}

	template auto Editor::LoadMeta(std::filesystem::path path)->meta::ModelMeta;

	template<typename T>
	auto Editor::LoadMeta(std::filesystem::path path) -> T {
		std::stringstream ss;
		auto buffer = kyanite::engine::core::LoadFileToBuffer(path.string());
		ss.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
		cereal::JSONInputArchive archive(ss);
		T meta;
		archive(meta);

		return meta;
	}

	template auto Editor::SaveBlob<kyanite::engine::rendering::ModelData>(
		std::string uuid,
		kyanite::engine::rendering::ModelData& data
	) -> void;

	template<typename T>
	auto Editor::SaveBlob(std::string uuid, T& data) -> void {
		std::stringstream ss;
		{
			cereal::BinaryOutputArchive archive(ss);
			archive(data);
		}

		kyanite::engine::logging::logger::Info("Saving blob: " + uuid);

		// Move string file data to buffer
		std::vector<uint8_t> buffer;
		buffer.resize(ss.str().size());
		auto str = ss.str();
		std::memcpy(buffer.data(), str.c_str(), str.size());
		auto blobPath = (_service->BlobsPath() / uuid.substr(0, 2) / uuid).string() + ".blob";
		kyanite::engine::core::SaveBufferToFile(blobPath, buffer);

		kyanite::engine::logging::logger::Info("Saved blob: " + uuid);
	}

	auto Editor::SaveModelMeta(std::string name, std::filesystem::directory_entry file) -> std::string {
		auto uuid = _assetDatabase->AddAsset(
			name,
			file.path().string(),
			assetpackages::AssetType::MODEL,
			file.last_write_time()
		);
		meta::ModelMeta meta;
		meta.uuid = uuid;

		SaveMeta<meta::ModelMeta>(name, file.path(), meta);

		return uuid;
	}

	auto Editor::SaveModelData(std::string uuid, std::vector<uint8_t>& data) -> void {
		auto modelBlob = _modelMapper->MapFbx(data);
		auto modelData = kyanite::engine::rendering::ModelData(modelBlob);

		SaveBlob<kyanite::engine::rendering::ModelData>(uuid, modelData);
	}

	auto Editor::SetupEditorEnvironment() -> void {
		CompareLastAssetDatabase();

		_sourceWatchdog = std::make_unique<FileWatchdog>(_service->SourcePath(), std::chrono::milliseconds(100));
		// TODO: Add support for source files
		_contentWatchdog = std::make_unique<FileWatchdog>(_service->ContentPath(), std::chrono::milliseconds(100));
		QObject::connect(_contentWatchdog.get(), &FileWatchdog::FileAdded, this, &Editor::HandleFileAdded);
		QObject::connect(_contentWatchdog.get(), &FileWatchdog::FileModified, this, &Editor::HandleFileModified);
		QObject::connect(_contentWatchdog.get(), &FileWatchdog::FileDeleted, this, &Editor::HandleFileRemoved);
		_contentWatchdog->Start();
	}

	auto Editor::OnFileAdded(std::filesystem::directory_entry file) -> void {
		// Do the work asynchronously so the UI doesn't freeze
		QFutureWatcher<void> watcher;

		auto dialog = ShowAssetProcessingDialog();
		QObject::connect(&watcher, &QFutureWatcher<void>::finished, [&]() {
			dialog->close();
			});
		watcher.connect(&watcher, &QFutureWatcher<void>::finished, dialog, &QProgressDialog::close);

		watcher.setFuture(QtConcurrent::run([&]() {
			HandleFileAdded(file);
			}));

		dialog->exec();
	}

	auto Editor::OnFileModified(std::filesystem::directory_entry file) -> void {
		// Do the work asynchronously so the UI doesn't freeze
		QFutureWatcher<void> watcher;

		auto dialog = ShowAssetProcessingDialog();
		QObject::connect(&watcher, &QFutureWatcher<void>::finished, [&]() {
			dialog->close();
			});
		watcher.connect(&watcher, &QFutureWatcher<void>::finished, dialog, &QProgressDialog::close);

		watcher.setFuture(QtConcurrent::run([&]() {
			HandleFileModified(file);
			}));

		dialog->exec();
	}

	auto Editor::OnFileRemoved(std::filesystem::path path) -> void {
		// Do the work asynchronously so the UI doesn't freeze
		QFutureWatcher<void> watcher;

		auto dialog = ShowAssetProcessingDialog();
		QObject::connect(&watcher, &QFutureWatcher<void>::finished, [&]() {
			dialog->close();
			});
		watcher.connect(&watcher, &QFutureWatcher<void>::finished, dialog, &QProgressDialog::close);

		watcher.setFuture(QtConcurrent::run([&]() {
			HandleFileRemoved(path);
			}));

		dialog->exec();
	}

	auto Editor::CompareLastAssetDatabase() -> void {
		// We check for each file in the content folder if it exists in the asset database
		// If it doesn't, we add it to the asset database
		// If it does, we check if the file has been modified
		// We also need to check if files in the asset database have been removed
		auto currentFiles = engine::core::GetFileList(_service->ContentPath());

		for (auto& file : currentFiles) {
			// First, check if the file is a meta file
			if (file.path().extension() == ".meta") {
				HandleMetaData(file);
				continue;
			}

			HandleData(file);
		}

		// Now we need to check if any files have been removed
		auto assetDatabaseFiles = _assetDatabase->GetAllAssets();
		for (auto& file : assetDatabaseFiles) {
			if (!engine::core::CheckIfFileExists(file.path)) {
				HandleFileRemoved(file.path);
			}
		}
	}

	auto Editor::HandleFileAdded(std::filesystem::directory_entry file) -> void {
		// Make sure the file is a regular file
		if (!file.is_regular_file()) {
			return;
		}

		// Do nothing if the file is a meta file
		if (file.path().extension() == ".meta") {
			return;
		}

		// We have a file, check if it's a valid asset type
		auto extension = file.path().extension().string();
		auto name = file.path().stem().string();

		auto buffer = engine::core::LoadFileToBuffer(file.path().string());

		if (extension == ".fbx") {
			auto uuid = SaveModelMeta(name, file);
			SaveModelData(uuid, buffer);
		}
	}

	auto Editor::HandleFileModified(std::filesystem::directory_entry file) -> void {
		// Make sure the file is a regular file
		if (!file.is_regular_file()) {
			return;
		}

		// Do nothing if the file is a meta file
		// TODO: Use the editor to change the meta and reimport the asset
		if (file.path().extension() == ".meta") {
			return;
		}

		// First we need to get the uuid from the meta file
		auto metaPath = file.path().string() + ".meta";
		std::string uuid = "";

		auto metaBuffer = engine::core::LoadFileToBuffer(metaPath);

		if (file.path().extension() == ".fbx") {
			auto meta = LoadMeta<meta::ModelMeta>(metaPath);
			uuid = meta.uuid;
		}

		// Update the asset database
		auto writeTime = std::filesystem::last_write_time(file.path());
		_assetDatabase->UpdateAsset(uuid, writeTime);

		// Update the blob 
		engine::core::RemoveFile(
			(_service->BlobsPath() / uuid.substr(0, 2) / uuid)
			.string() + ".blob"
		);

		auto buffer = engine::core::LoadFileToBuffer(file.path().string());
		SaveModelData(uuid, buffer);
	}

	auto Editor::HandleFileRemoved(std::filesystem::path file) -> void {
		// Meta data was removed, just reimport the asset
		if (file.extension() == ".meta") {
			// Delete the asset via its path since we don't have the uuid anymore
			// Get the uuid from the asset database
			auto filePath = file.parent_path() / file.stem();
			auto uuid = _assetDatabase->GetUuidForPath(filePath.string());
			// Now we need to remove the asset from the asset database
			_assetDatabase->RemoveAsset(uuid);
			// Now we need to remove the blob
			if (engine::core::CheckIfFileExists((_service->BlobsPath() / uuid.substr(0, 2) / uuid).string() + ".blob")
			) {
				engine::core::RemoveFile((_service->BlobsPath() / uuid.substr(0, 2) / uuid).string() + ".blob");
			}

			// Reimport the asset if it exists
			if (engine::core::CheckIfFileExists((file.parent_path() / file.stem()).string())) {
				HandleFileAdded(std::filesystem::directory_entry((file.parent_path() / file.stem()).string()));
			}
		}
		else {
			std::string uuid = _assetDatabase->GetUuidForPath(file.string());
			// Remove the asset from the asset database
			_assetDatabase->RemoveAsset(uuid);
			// Also remove the blob
			engine::core::RemoveFile((_service->BlobsPath() / uuid.substr(0, 2) / uuid).string() + ".blob");

			// Also remove the meta file if it exists
			auto metaPath = file.string() + ".meta";
			if (engine::core::CheckIfFileExists(metaPath)) {
				engine::core::RemoveFile(metaPath);
			}
		}
	}

	auto Editor::HandleMetaData(std::filesystem::directory_entry file) -> void {
		// Ensure the meta file has a corresponding file
		auto filePath = (file.path().parent_path() / file.path().stem());
		if (!engine::core::CheckIfFileExists(filePath.string())) {
			// Get the asset type so we can get the uuid
			std::string uuid = "";
			if (filePath.extension() == ".fbx") {
				auto meta = LoadMeta<meta::ModelMeta>(file.path());
				uuid = meta.uuid;
			}

			engine::core::RemoveFile(file.path().string());

			HandleFileRemoved(filePath);
		}
	}

	auto Editor::HandleData(std::filesystem::directory_entry file) -> void {
		// Check if file has a meta file
		auto metaPath = file.path().string() + ".meta";
		if (engine::core::CheckIfFileExists(metaPath)) {
			std::string uuid = "";
			if (file.path().extension() == ".fbx") {
				auto meta = LoadMeta<meta::ModelMeta>(metaPath);
				uuid = meta.uuid;
			}

			// The editor assetdatabase uses uuids instead of the path behind the scenes
			// Package is also not used because there is only one database

			// We know the file exists so we need to see if it was modified
			if (_assetDatabase->CheckIfPackageHasAsset(nullptr, uuid)) {
				// Check if the file has been modified
				auto lastModified = _assetDatabase->GetModifiedTime(uuid);
				auto currentModified = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::filesystem::last_write_time(file.path()).time_since_epoch()
				).count();

				if (lastModified != currentModified) {
					// File has been modified, update the asset database
					HandleFileModified(file);
				}
			}
			else {
				// File was not found in the asset database, add it
				HandleFileAdded(file);
			}
		}
		else {
			// File has no meta file, add it
			HandleFileAdded(file);
		}
	}

	auto Editor::ShowAssetProcessingDialog() -> QProgressDialog* {
		auto dialog = new QProgressDialog("Processing assets...", "Cancel", 0, 0);
		dialog->setWindowModality(Qt::WindowModal);
		dialog->setMinimumDuration(0);
		dialog->setCancelButton(nullptr);
		dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
		dialog->setWindowFlag(Qt::WindowStaysOnTopHint, true);
		dialog->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
		dialog->setWindowFlag(Qt::WindowCloseButtonHint, false);
		dialog->setWindowFlag(Qt::WindowMinimizeButtonHint, false);
		dialog->setWindowFlag(Qt::WindowMaximizeButtonHint, false);
		dialog->setWindowFlag(Qt::WindowFullscreenButtonHint, false);

		return dialog;
	}
}