#include "assetpipeline/Bridge_AssetPipeline.h"
#include "assetpipeline/loaders/FbxLoader.hxx"

#include "shared/Result.hxx"
#include <logger/Logger.hxx>
#include <logger/ConsoleLogger.hxx>

#include <windows.h>

namespace assetpipeline = kyanite::editor::assetpipeline;

EXPORTED void AssetPipeline_Init() {

}

EXPORTED Result AssetPipeline_LoadMeshes(const char* path, MeshData** meshData, size_t* len) {
	try {
		OutputDebugStringA(path);
		auto meshes = assetpipeline::loaders::FbxLoader().LoadFromFile(path);
		OutputDebugStringA("Loaded meshes");
		auto meshCount = meshes.size();
		OutputDebugStringA("Got mesh count");
		OutputDebugStringA(std::to_string(meshCount).c_str());
		auto rawMeshPtr = new MeshData[meshCount];
		for (size_t i = 0; i < meshCount; i++) {
			rawMeshPtr[i].vertexCount = meshes[i].vertices.size();
			rawMeshPtr[i].indexCount = meshes[i].indices.size();
			rawMeshPtr[i].vertices = new Vertex[meshes[i].vertices.size()];
			rawMeshPtr[i].indices = new uint32_t[meshes[i].indices.size()];

			memcpy(rawMeshPtr[i].vertices, meshes[i].vertices.data(), sizeof(Vertex) * meshes[i].vertices.size());
			memcpy(rawMeshPtr[i].indices, meshes[i].indices.data(), sizeof(uint32_t) * meshes[i].indices.size());
		}

		*meshData = rawMeshPtr;
		*len = meshCount;

		return Result_OK();

	} catch(std::runtime_error& e) {
		*len = 0;
		*meshData = nullptr;
		return Result_Error(e.what());
	}
}

EXPORTED void AssetPipeline_FreeMeshes(MeshData* meshData, size_t len) {
	for (size_t i = 0; i < len; i++) {
		delete[] meshData[i].vertices;
		delete[] meshData[i].indices;
	}
	delete[] meshData;
}