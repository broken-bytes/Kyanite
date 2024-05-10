#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>
#include <shared/Result.hxx>

#ifdef __cplusplus
extern "C" {
#endif
	EXPORTED struct Vertex {
		float position[3];
		float normal[3];
		float texCoord[2];
	} typedef Vertex;

	EXPORTED struct MeshData {
		const char* name;
		Vertex* vertices;
		uint32_t vertexCount;
		uint32_t* indices;
		uint32_t indexCount;
	} typedef MeshData;

    EXPORTED struct AnimationClipData {
        const char* name;
        const char* takeName;
        int startFrame;
        int endFrame;
        float frameRate;
    } typedef AnimationClipData;

    EXPORTED struct AnimationData {
		AnimationClipData* clips;
		size_t clipCount;
    } typedef AnimationData;

	EXPORTED struct ModelData {
		const char* name;
		MeshData* meshes;
		size_t meshCount;
		AnimationData* animations;
	} typedef ModelData;

	/**
	* @brief Initialize the asset pipeline
	*/
	EXPORTED void AssetPipeline_Init();

	/**
	* @brief Load a mesh from a buffer
	* @param path The buffet to load from
	* @param meshData The mesh data to load into
	* @param len The length of the mesh data
	*/
	EXPORTED Result AssetPipeline_LoadMeshes(const char* path, struct MeshData** meshData, size_t* len);

	/**
	* @brief Free the mesh data
	* @param meshData The mesh data to free
	*/
	EXPORTED void AssetPipeline_FreeMeshes(struct MeshData* meshData, size_t len);


#ifdef __cplusplus
}
#endif
