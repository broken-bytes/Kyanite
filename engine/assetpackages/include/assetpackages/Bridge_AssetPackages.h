#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>

#ifdef __cplusplus 
extern "C" {
#endif
	/**
	* @brief Initializes the asset packages
	* @param loader The loader to use. Creates the engine's internal one if none provided.
	*/
	EXPORTED void AssetPackages_Initialize(NativePointer loader);

	/**
	* @brief Loads an asset package
	* @param path The path to the asset package
	* @return The asset package
	*/
	EXPORTED void AssetPackages_LoadAssetPackages(const char* path, NativePointer* buffer, size_t* numPackages);

	/**
	* @brief Loads a texture
	* @param uuid The uuid of the texture
	* @return The texture
	*/
	EXPORTED NativePointer AssetPackages_LoadTexture(NativePointer assetPackage, const char* uuid);

	/**
	 * @brief Clears a native pointer
	 * @param pointer The pointer to clear
	 */
	EXPORTED void AssetPackages_FreeTexture(NativePointer pointer);

	/**
	* @brief Loads a model
	* @param uuid The uuid of the model
	* @param array of ids of the meshes
	* @param size of the array
	*/
	EXPORTED void AssetPackages_LoadModel(NativePointer assetPackage, const char* uuid, uint8_t** ids, size_t* numMeshes);

	/**
	* @brief Loads a mesh
	* @param uuid The uuid of the model
	* @param id The id of the mesh
	* @param vertices The vertices of the mesh
	* @param indices The indices of the mesh
	*/
	EXPORTED void AssetPackages_LoadMesh(
		NativePointer assetPackage, 
		const char* uuid, 
		uint8_t id, 
		float** vertices, 
		size_t* numVertices,
		uint32_t** indices,
		size_t* numIndices
	);

	/**
	 * @brief Clears a native pointer
	 * @param pointer The pointer to clear
	 */
	EXPORTED void AssetPackages_FreeMesh(NativePointer pointer);

	/**
	* @brief Loads a material
	* @param uuid The uuid of the material
	* @return The material
	*/
	EXPORTED NativePointer AssetPackages_LoadMaterial(NativePointer assetPackage, const char* uuid);

	/**
	 * @brief Clears a native pointer
	 * @param pointer The pointer to clear
	 */
	EXPORTED void AssetPackages_FreeMaterial(NativePointer pointer);

	/**
	*	@brief Loads a shader
	* @param uuid The uuid of the shader
	* @return The shader
	*/
	EXPORTED NativePointer AssetPackages_LoadShader(NativePointer assetPackage, const char* uuid);

	/**
	 * @brief Clears a native pointer
	 * @param pointer The pointer to clear
	 */
	EXPORTED void AssetPackages_FreeShader(NativePointer pointer);

	/**
	* @brief Loads an audioclip
	* @param uuid The uuid of the audioclip
	* @return The audioclip
	*/
	EXPORTED NativePointer AssetPackages_LoadAudioClip(NativePointer assetPackage, const char* uuid);

	/**
	 * @brief Clears a native pointer
	 * @param pointer The pointer to clear
	 */
	EXPORTED void AssetPackages_FreeAudioClip(NativePointer pointer);

	/**
	* @brief Clears an asset
	* @param asset The asset to clear
	*/
	EXPORTED void AssetPackages_DisposeAsset(NativePointer asset);

#ifdef __cplusplus 
}
#endif