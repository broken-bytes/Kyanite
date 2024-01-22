#include "editor/EditorScreenViewModel.hxx"

#include <core/Core.hxx>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <sstream>

namespace kyanite::editor {
	EditorScreenViewModel::EditorScreenViewModel(
		std::unique_ptr<ProjectService> service
	) : _service(std::move(service)) {

	}

	EditorScreenViewModel::~EditorScreenViewModel() {

	}

	auto EditorScreenViewModel::LoadProject(const std::string& path) -> Project {
		return _service->LoadProject(path);
	}

	auto EditorScreenViewModel::CreateProject(const std::string& path, std::string& name) -> Project {
		return _service->CreateProject(path, name);
	}
}