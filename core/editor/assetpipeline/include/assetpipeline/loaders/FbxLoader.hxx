#pragma once

#include <rendering/Mesh.hxx>
#include <cstdint>
#include <tuple>
#include <vector>

namespace kyanite::editor::assetpipeline::loaders {
	class FbxLoader {
	public:
		FbxLoader();
		~FbxLoader();
		auto LoadFromBuffer(std::vector<uint8_t> buffer) -> std::vector<kyanite::engine::rendering::MeshData>;
	};
}