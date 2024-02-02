#pragma once

#include "../meta/ModelMeta.hxx"
#include <rendering/Mesh.hxx>

#include <cstdint>
#include <tuple>
#include <vector>

namespace kyanite::editor::loaders {
	class FbxLoader {
	public:
		FbxLoader();
		~FbxLoader();
		auto LoadFromBuffer(std::vector<uint8_t> buffer) -> std::vector<engine::rendering::MeshData>;
	};
}