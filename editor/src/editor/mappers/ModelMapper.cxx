#include "editor/mappers/ModelMapper.hxx"
#include "editor/meta/ModelMeta.hxx"
#include "editor/loaders/FbxLoader.hxx"

#include <core/Logger.hxx>
#include <rendering/Mesh.hxx>
#include <rendering/Vertex.hxx>

#include <vector>

namespace rendering = kyanite::engine::rendering;

auto kyanite::editor::mappers::ModelMapper::MapFbx(
	std::vector<uint8_t> data
) -> std::vector<kyanite::engine::rendering::MeshData> {
	auto loader = kyanite::editor::loaders::FbxLoader();

	return loader.LoadFromBuffer(data);
}