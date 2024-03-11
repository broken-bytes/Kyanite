#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* @brief Adds an asset to the database.
	* @param database The database to add the asset to.
	* @param uuid The UUID of the asset.
	* @param name The name of the asset.
	* @param path The path of the asset.
	* @param type The type of the asset.
	* @param time The time the asset was last modified.
	* @return True if the asset was added, false if the asset already exists.
	*/
	int AssetDatabase_AddAsset(
		void* database,
		const char* uuid,
		const char* name,
		const char* path,
		const char* type,
		int64_t time
	);

	/**
	* @brief Updates the asset in the database.
	* @param database The database to update the asset in.
	* @param uuid The UUID of the asset.
	* @param time The time the asset was last modified.
	*/
	void AssetDatabase_UpdateAsset(void* database, const char* uuid, int64_t time);

	/**
	* @brief Removes an asset from the database.
	* @param database The database to remove the asset from.
	* @param uuid The UUID of the asset.
	*/
	void AssetDatabase_RemoveAsset(void* database, const char* uuid);

	/**
	* @brief Gets the last modified time of an asset.
	* @param database The database to get the asset from.
	* @param uuid The UUID of the asset.
	* @return The last modified time of the asset.
	*/
	int64_t AssetDatabase_GetModifiedTime(void* database, const char* uuid);

	/**
	* @brief Gets the UUID of an asset by its path.
	* @param database The database to get the asset from.
	* @param path The path of the asset.
	* @param uuid The UUID of the asset. If the asset does not exist, this will be set to NULL.
	*/
	void AssetDatabase_GetUuidForPath(void* database, const char* path, const char** uuid);

	/**
	* @brief Deallocates a UUID.
	* @param uuid The UUID to deallocate.
	*/
	void AssetDatabase_FreeUuid(const char* uuid);

	/**
	* @brief Creates or loads an asset database.
	* @param path The path of the database.
	* @return The database.
	*/
	void* AssetDatabase_CreateOrLoad(const char* path);

	/**
	* @brief Closes an asset database.
	* @param database The database to close.
	*/
	void AssetDatabase_Close(void* database);


#ifdef __cplusplus
}
#endif
