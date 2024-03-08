#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#ifdef __cplusplus
extern "C" {
#endif
	struct Vertex {
		float position[3];
		float normal[3];
		float texCoord[2];
	} typedef Vertex;

	struct MeshData {
		const char* name;
		Vertex* vertices;
		uint32_t vertexCount;
		uint32_t* indices;
		uint32_t indexCount;
	} typedef MeshData;

	/**
	* @brief Initialize the asset pipeline
	*/
	EXPORTED void AssetPipeline_Init();

	/**
	* @brief Load a mesh from a file
	* @param path The path to the file
	* @param meshData The mesh data to load into
	* @param len The length of the mesh data
	*/
	EXPORTED void AssetPipeline_LoadMeshes(const uint8_t* buffer, size_t bufferLen, MeshData* meshData, size_t* len);

	/**
	* @brief Free the mesh data
	* @param meshData The mesh data to free
	*/
	EXPORTED void AssetPipeline_FreeMeshes(MeshData* meshData, size_t len);


#ifdef __cplusplus
}
#endif
