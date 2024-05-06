#include "io/Bridge_IO.h"
#include "io/IO.hxx"

#include <shared/NativePointer.hxx>

#include <SDL2/SDL.h>
#include <iostream>

int8_t IO_CreateArchive(const char* path) {
	return kyanite::engine::io::CreateArchive(path);
}

int8_t IO_CheckIfFileExistsInArchive(const char* archivePath, const char* filePath) {
	return kyanite::engine::io::CheckIfFileExists(archivePath, filePath);
}

int8_t IO_SaveBufferToArchive(const char* path, const char* name, const char* buffer) {
	return kyanite::engine::io::SaveBufferToArchive(path, name, buffer);
}

int8_t IO_LoadFileFromArchive(const char* path, const char* name, uint8_t** buffer, size_t* size) {
	return kyanite::engine::io::LoadFileFromArchive(path, name, buffer, size);
}
