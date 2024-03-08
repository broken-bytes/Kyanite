#include "assetpipeline/Bridge_AssetPipeline.h"
#include "assetpipeline/loaders/FbxLoader.hxx"

namespace assetpipeline = kyanite::editor::assetpipeline;

EXPORTED void AssetPipeline_Init() {

}

EXPORTED void AssetPipeline_LoadMeshes(const uint8_t* buffer, size_t bufferLen, MeshData* meshData, size_t* len) {
	auto vec = std::vector<uint8_t>(buffer, buffer + bufferLen);
	auto meshes = assetpipeline::loaders::FbxLoader().LoadFromBuffer(vec);

	*len = meshes.size();
	meshData = new MeshData[*len];
	for (size_t i = 0; i < *len; i++) {
		meshData[i].vertexCount = meshes[i].vertices.size();
		meshData[i].indexCount = meshes[i].indices.size();
		meshData[i].vertices = new Vertex[meshData[i].vertexCount];
		meshData[i].indices = new uint32_t[meshData[i].indexCount];

		memcpy(meshData[i].vertices, meshes[i].vertices.data(), sizeof(Vertex) * meshData[i].vertexCount);
		memcpy(meshData[i].indices, meshes[i].indices.data(), sizeof(uint32_t) * meshData[i].indexCount);
	}
}

EXPORTED void AssetPipeline_FreeMeshes(MeshData* meshData, size_t len) {
	for (size_t i = 0; i < len; i++) {
		delete[] meshData[i].vertices;
		delete[] meshData[i].indices;
	}
	delete[] meshData;
}