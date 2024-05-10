#pragma once

#include "assetpipeline/mappers/IMapper.hxx"
#include "assetpipeline/Bridge_AssetPipeline.h"
#include <rendering/Mesh.hxx>

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

namespace kyanite::editor::assetpipeline::mappers {
	class ModelMapper {
	public:
		ModelMapper() = default;

		auto MapFbx(
			const char* path
		) -> std::vector<kyanite::engine::rendering::MeshData>;
	};
}