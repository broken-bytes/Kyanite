#include "editor/EditorScreenViewModel.hxx"
#include <engine/Bridge_Engine.h>
#include <core/Core.hxx>
#include <core/VendorSerializers/GlmSerializers.hxx>
#include <rendering/Model.hxx>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <sstream>
#include <vector>

namespace assetpackages = kyanite::engine::assetpackages;

namespace kyanite::editor {
	EditorScreenViewModel::EditorScreenViewModel(
		std::unique_ptr<ProjectService> service,
		std::unique_ptr<AssetDatabase> assetDatabase
	) : _service(std::move(service)), _assetDatabase(std::move(assetDatabase)) {

		_modelMapper = std::make_unique<mappers::ModelMapper>();
	}

	EditorScreenViewModel::~EditorScreenViewModel() {

	}

	auto EditorScreenViewModel::LoadProject(const std::string& path) -> Project {
		auto project = _service->LoadProject(path);

		return project;
	}

	auto EditorScreenViewModel::CreateProject(const std::string& path, std::string& name) -> Project {
		auto project = _service->CreateProject(path, name);

		return project;
	}

	auto EditorScreenViewModel::InitializeEngine(WId window) -> void {
		Bridge_Engine_Init(
			reinterpret_cast<NativePointer>(window),
			reinterpret_cast<NativePointer>(_assetDatabase.get()),
			EDITOR,
			nullptr
		);

		_assetDatabase->Load(_service->CachePath());
	}

	template auto EditorScreenViewModel::SaveMeta<meta::ModelMeta>(
		std::string name, 
		std::filesystem::path path, 
		meta::ModelMeta& meta
	) -> void;

	template<typename T>
	auto EditorScreenViewModel::SaveMeta(std::string name, std::filesystem::path path, T& meta) -> void {
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

	template auto EditorScreenViewModel::LoadMeta(std::filesystem::path path)->meta::ModelMeta;

	template<typename T>
	auto EditorScreenViewModel::LoadMeta(std::filesystem::path path) -> T {
		std::stringstream ss;
		auto buffer = kyanite::engine::core::LoadFileToBuffer(path.string());
		ss.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
		cereal::JSONInputArchive archive(ss);
		T meta;
		archive(meta);

		return meta;
	}

	template auto EditorScreenViewModel::SaveBlob<kyanite::engine::rendering::ModelData>(
		std::string uuid,
		kyanite::engine::rendering::ModelData& data
	) -> void;

	template<typename T>
	auto EditorScreenViewModel::SaveBlob(std::string uuid, T& data) -> void {
		std::stringstream ss;
		{
			cereal::BinaryOutputArchive archive(ss);
			archive(data);
		}

		// Move string file data to buffer
		std::vector<uint8_t> buffer;
		buffer.resize(ss.str().size());
		auto str = ss.str();
		std::memcpy(buffer.data(), str.c_str(), str.size());
		auto blobPath = (_service->BlobsPath() / uuid.substr(0, 2) / uuid).string() + ".blob";
		kyanite::engine::core::SaveBufferToFile(blobPath, buffer);
	}

	auto EditorScreenViewModel::SaveModelMeta(std::string name, std::filesystem::directory_entry file) -> std::string {
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

	auto EditorScreenViewModel::SaveModelData(std::string uuid, std::vector<uint8_t>& data) -> void {
		auto modelBlob = _modelMapper->MapFbx(data);
		auto modelData = kyanite::engine::rendering::ModelData(modelBlob);

		SaveBlob<kyanite::engine::rendering::ModelData>(uuid, modelData);
	}

	auto EditorScreenViewModel::SetupEditorEnvironment() -> void {
		CompareLastAssetDatabase();

		_sourceWatchdog = std::make_unique<FileWatchdog>(_service->SourcePath(), std::chrono::milliseconds(100));
		_sourceWatchdog->SetCallback([this](FileEvent event, std::filesystem::directory_entry file) {
			switch (event) {
			case FileEvent::ADDED:
				break;
			case FileEvent::DELETED:
				break;
			case FileEvent::MODIFIED:
				break;
			}
			});
		_sourceWatchdog->Start();

		_contentWatchdog = std::make_unique<FileWatchdog>(_service->ContentPath(), std::chrono::milliseconds(100));
		_contentWatchdog->SetCallback([this](FileEvent event, std::filesystem::directory_entry file) {
			switch (event) {
			case FileEvent::ADDED:
				// Add to asset database and generate meta file
				if (file.is_regular_file()) {
					// Handle file added
					HandleFileAdded(file);
				}
				break;
			case FileEvent::DELETED:
				if (file.is_regular_file()) {
					HandleFileRemoved(file.path());
				}
				break;
			case FileEvent::MODIFIED:
				if (file.is_regular_file()) {
					HandleFileModified(file);
				}
				break;
			}
			});
		_contentWatchdog->Start();
	}

	auto EditorScreenViewModel::CompareLastAssetDatabase() -> void {
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
		for(auto& file : assetDatabaseFiles) {
			if (!engine::core::CheckIfFileExists(file.path)) {
				HandleFileRemoved(file.path);
			}
		}
	}

	auto EditorScreenViewModel::HandleFileAdded(std::filesystem::directory_entry file) -> void {
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

	auto EditorScreenViewModel::HandleFileModified(std::filesystem::directory_entry file) -> void {
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
		engine::core::RemoveFile((_service->BlobsPath() / uuid.substr(0, 2) / uuid).string() + ".blob");

		auto buffer = engine::core::LoadFileToBuffer(file.path().string());
		SaveModelData(uuid, buffer);
	}

	auto EditorScreenViewModel::HandleFileRemoved(std::filesystem::path file) -> void {
		// Meta data was removed, just reimport the asset
		if (file.extension() == ".meta") {
			// Delete the asset via its path since we don't have the uuid anymore
			// Get the uuid from the asset database
			auto filePath = file.parent_path() / file.stem();
			auto uuid = _assetDatabase->GetUuidForPath(filePath.string());
			// Now we need to remove the asset from the asset database
			_assetDatabase->RemoveAsset(uuid);
			// Now we need to remove the blob
			if (engine::core::CheckIfFileExists((_service->BlobsPath() / uuid.substr(0, 2) / uuid).string() + ".blob")) {
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

	auto EditorScreenViewModel::HandleMetaData(std::filesystem::directory_entry file) -> void {
		// Ensure the meta file has a corresponding file
		auto filePath = (file.path().parent_path() / file.path().stem());
		if (!engine::core::CheckIfFileExists(filePath.string())) {
			// Get the asset type so we can get the uuid
			std::string uuid = "";
			if(filePath.extension() == ".fbx") {
				auto meta = LoadMeta<meta::ModelMeta>(file.path());
				uuid = meta.uuid;
			}

			engine::core::RemoveFile(file.path().string());

			HandleFileRemoved(filePath);
		}
	}

	auto EditorScreenViewModel::HandleData(std::filesystem::directory_entry file) -> void {
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
				auto currentModified = std::filesystem::last_write_time(file.path());

				if (lastModified != currentModified.time_since_epoch().count()) {
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
}