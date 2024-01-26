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
		SetupEditorEnvironment();

		return project;
	}

	auto EditorScreenViewModel::CreateProject(const std::string& path, std::string& name) -> Project {
		auto project = _service->CreateProject(path, name);
		SetupEditorEnvironment();

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

	template auto EditorScreenViewModel::SaveMeta<meta::ModelMeta>(std::string name, std::filesystem::path path, meta::ModelMeta& meta) -> void;

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

		auto metaPath = (path.parent_path() / name).string() + ".meta";
		kyanite::engine::core::SaveBufferToFile(metaPath, buffer);
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

	auto EditorScreenViewModel::SetupEditorEnvironment() -> void {
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
					// We have a file, check if it's a valid asset type
					auto extension = file.path().extension().string();
					auto name = file.path().stem().string();

					if (extension == ".fbx") {
						auto uuid = _assetDatabase->AddAsset(name, file.path().string(), AssetType::MODEL);
						auto fbxBuffer = kyanite::engine::core::LoadFileToBuffer(file.path().string());
						auto data = _modelMapper->MapFbx(fbxBuffer);
						data.first.uuid = uuid;
						data.first.name = name;

						SaveMeta<meta::ModelMeta>(name, file.path(), data.first);
						auto modelData = kyanite::engine::rendering::ModelData(data.second);
						SaveBlob<kyanite::engine::rendering::ModelData>(uuid, modelData);
					}
				}
				break;
			case FileEvent::DELETED:
				break;
			case FileEvent::MODIFIED:
				break;
			}
			});
		_contentWatchdog->Start();
	}
}