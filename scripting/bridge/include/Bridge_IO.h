#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#ifdef __cplusplus 
extern "C" {
#endif
	/**
	 * @brief Creates an archive
	 *
	 * @param path The path to the archive
	 */
	EXPORTED int8_t IO_CreateArchive(
		const char* path
	);

	/**
	 * @brief Checks if a file exists in an archive
	 *
	 * @param archivePath The path to the archive
	 * @param filePath The path to the file
	 */
	EXPORTED int8_t IO_CheckIfFileExistsInArchive(
		const char* archivePath,
		const char* filePath
	);

	/**
	* @brief Saves a buffer to an archive
	*
	* @param path The path to the archive
	* @param name The name of the file
	* @param buffer The buffer to save
	*/
	EXPORTED int8_t IO_SaveBufferToArchive(
		const char* path,
		const char* name,
		const char* buffer
	);

	/**
	 * @brief Loads a file from an archive
	 *
	 * @param path The path to the archive
	 * @param name The name of the file
	 * @param buffer The buffer to save
	 * @param size The size of the buffer
	 */
	EXPORTED int8_t IO_LoadFileFromArchive(
		const char* path,
		const char* name,
		uint8_t** buffer,
		size_t* size
	);
#ifdef __cplusplus 
}
#endif
