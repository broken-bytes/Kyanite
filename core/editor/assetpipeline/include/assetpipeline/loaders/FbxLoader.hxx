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
		auto LoadFromFile(
			const char* path
		) -> std::vector<kyanite::engine::rendering::MeshData>;
	};
}