#include "editor/EditorScreenViewModel.hxx"
#include <engine/Bridge_Engine.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <sstream>

namespace assetpackages = kyanite::engine::assetpackages;

namespace kyanite::editor {
	EditorScreenViewModel::EditorScreenViewModel(
		std::unique_ptr<ProjectService> service,
		std::unique_ptr<AssetDatabase> assetDatabase
	) : _service(std::move(service)), _assetDatabase(std::move(assetDatabase)) {
	}

	EditorScreenViewModel::~EditorScreenViewModel() {

	}

	auto EditorScreenViewModel::LoadProject(const std::string& path) -> Project {
		return _service->LoadProject(path);
	}

	auto EditorScreenViewModel::CreateProject(const std::string& path, std::string& name) -> Project {
		return _service->CreateProject(path, name);
	}

	auto EditorScreenViewModel::InitializeEngine(WId window) -> void {
		Bridge_Engine_Init(
			reinterpret_cast<NativePointer>(window), 
			reinterpret_cast<NativePointer>(_assetDatabase.get())
		);

		_assetDatabase->Load(_service->CachePath());
		_assetDatabase->AddAsset("default", "default.kap");
	}
}