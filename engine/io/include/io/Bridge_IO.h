#pragma once

#include "IOSystem.hxx"
#include "FileHandle.hxx"

#include <core/Exported.hxx>
#include <core/NativePointer.hxx>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	EXPORTED void IO_Init();
	EXPORTED NativePointer IO_OpenFile(const char* path);
	EXPORTED void IO_CloseFile(NativePointer file);
	EXPORTED uint8_t* IO_ReadFile(NativePointer file, uint64_t& numBytesRead);
	EXPORTED uint8_t* IO_ReadFileNum(NativePointer file, uint64_t numBytes, uint64_t& numBytesRead);
	EXPORTED void IO_WriteFile(NativePointer file, const uint8_t* bytes, uint64_t numBytes);

	/**
	 * @brief Inits the input system
	 *
	 */
	void IO_Init() {
		io::Init();
	}

	/**
	 * @brief Opens a file
	 *
	 * @param path The path of the file
	 * @return NativePointer The file handle
	 */
	NativePointer IO_OpenFile(const char* path) {
		return reinterpret_cast<NativePointer>(io::OpenFile(path));
	}

	/**
	 * @brief Closes a file
	 *
	 * @param file The file handle
	 */
	void IO_CloseFile(NativePointer file) {
		delete reinterpret_cast<io::FileHandle*>(file);
	}

	/**
	 * @brief Reads a file
	 *
	 * @param file The file handle
	 * @param numBytesRead The number of bytes read
	 * @return uint8_t* The bytes read
	 */
	uint8_t* IO_ReadFile(NativePointer file, uint64_t& numBytesRead) {
		return IO_ReadFileNum(file, UINT64_MAX, numBytesRead);
	}

	/**
	 * @brief Reads a file
	 *
	 * @param file The file handle
	 * @param numBytes The number of bytes to read
	 * @param numBytesRead The number of bytes read
	 * @return uint8_t* The bytes read
	 */
	uint8_t* IO_ReadFileNum(NativePointer file, uint64_t numBytes, uint64_t& numBytesRead) {
		auto data = reinterpret_cast<io::FileHandle*>(file)->Read(numBytes);
		auto bytes = new uint8_t[data.size()];
		memcpy(bytes, data.data(), data.size());

		return bytes;
	}

	/**
	 * @brief Writes to a file
	 *
	 * @param file The file handle
	 * @param bytes The bytes to write
	 * @param numBytes The number of bytes to write
	 */
	void IO_WriteFile(NativePointer file, const uint8_t* bytes, uint64_t numBytes) {
		std::vector<std::byte> data(numBytes);
		memcpy(data.data(), bytes, numBytes);
		reinterpret_cast<io::FileHandle*>(file)->Write(data);
	}

#ifdef __cplusplus
}
#endif