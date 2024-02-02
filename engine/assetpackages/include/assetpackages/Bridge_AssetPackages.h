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
	EXPORTED NativePointer AssetPackages_LoadAssetPackages(const char* path, size_t* numPackages);

	/**
	* @brief Loads a texture
	* @param uuid The uuid of the texture
	* @return The texture
	*/
	EXPORTED NativePointer AssetPackages_LoadTexture(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Loads a mesh
	* @param uuid The uuid of the mesh
	* @return The mesh
	*/
	EXPORTED NativePointer AssetPackages_LoadMesh(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Loads a material
	* @param uuid The uuid of the material
	* @return The material
	*/
	EXPORTED NativePointer AssetPackages_LoadMaterial(NativePointer assetPackage, const char* uuid);

	/**
	*	@brief Loads a shader
	* @param uuid The uuid of the shader
	* @return The shader
	*/
	EXPORTED NativePointer AssetPackages_LoadShader(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Loads an audioclip
	* @param uuid The uuid of the audioclip
	* @return The audioclip
	*/
	EXPORTED NativePointer AssetPackages_LoadAudioClip(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Clears an asset
	* @param asset The asset to clear
	*/
	EXPORTED void AssetPackages_DisposeAsset(NativePointer asset);

    #ifdef __cplusplus 
}
#endif