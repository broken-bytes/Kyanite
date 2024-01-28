#pragma once

#include "editor/mappers/IMapper.hxx"
#include "meta/ModelMeta.hxx"
#include <rendering/Mesh.hxx>

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

namespace kyanite::editor::mappers {
	class ModelMapper {
	public:
		ModelMapper() = default;

		auto MapFbx(
			std::vector<uint8_t> data
		)-> std::vector<kyanite::engine::rendering::MeshData>;
	};
}