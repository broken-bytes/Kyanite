#include "assetpipeline/mappers/ModelMapper.hxx"
#include "assetpipeline/loaders/FbxLoader.hxx"

#include <logger/Logger.hxx>
#include <rendering/Mesh.hxx>
#include <rendering/Vertex.hxx>

#include <vector>

namespace rendering = kyanite::engine::rendering;

auto kyanite::editor::assetpipeline::mappers::ModelMapper::MapFbx(
	std::vector<uint8_t> data
) -> std::vector<kyanite::engine::rendering::MeshData> {
	auto loader = kyanite::editor::assetpipeline::loaders::FbxLoader();

	try {
		return loader.LoadFromBuffer(data);
	} catch (const std::exception& e) {
		kyanite::engine::logging::logger::Error(e.what());
		return {};
	}
}