#pragma once

#include <shared/Exported.hxx>

#include <imgui.h>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace kyanite::engine::io {
    EXPORTED auto LoadFileToBuffer(std::string_view path) -> std::vector<uint8_t>;
    EXPORTED auto SaveBufferToFile(std::string_view path, std::vector<uint8_t>& buffer) -> void;
    EXPORTED auto CreateDirectory(std::string_view path) -> void;
    EXPORTED auto CreateArchive(const char* path) -> int8_t;
    EXPORTED auto OpenArchive(std::string path) -> void;
    EXPORTED auto CreateFile(std::string path) -> void;
    EXPORTED auto RemoveFile(std::string path) -> void;
    EXPORTED auto GetFileList(std::filesystem::path path) -> std::vector<std::filesystem::directory_entry>;
    EXPORTED auto CheckIfFileExists(std::string path) -> int8_t;
    EXPORTED auto CheckIfFileExists(std::string archivePath, std::string filePath) -> int8_t;
    EXPORTED auto SaveBufferToArchive(const char* path, const char* name, const char* buffer) -> int8_t;
    EXPORTED auto LoadFileFromArchive(
        const char* path,
        const char* name,
        uint8_t** buffer,
        size_t* len
    ) -> int8_t;
    EXPORTED auto LoadFileFromArchivePartial(
        const char* path, 
        const char* name,
        size_t start,
        size_t count,
        uint8_t** buffer,
        size_t* len
    ) -> int8_t;
    EXPORTED auto CreateUUID() -> std::string;
}
