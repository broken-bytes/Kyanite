#pragma once

#include <shared/Exported.hxx>
#include <core/ILogger.hxx>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace kyanite::engine::core {
    EXPORTED auto InitCore() -> void;
    EXPORTED auto LoadFileToBuffer(std::string_view path) -> std::vector<uint8_t>;
    EXPORTED auto SaveBufferToFile(std::string_view path, std::vector<uint8_t>& buffer) -> void;
    EXPORTED auto CreateDirectory(std::string_view path) -> void;
    EXPORTED auto CreateArchive(std::string_view path) -> void;
    EXPORTED auto OpenArchive(std::string path) -> void;
    EXPORTED auto CreateFile(std::string path) -> void;
    EXPORTED auto RemoveFile(std::string path) -> void;
    EXPORTED auto GetFileList(std::filesystem::path path) -> std::vector<std::filesystem::directory_entry>;
    EXPORTED auto CheckIfFileExists(std::string path) -> bool;
    EXPORTED auto CheckIfFileExists(std::string archivePath, std::string filePath) -> bool;
    EXPORTED auto SaveBufferToArchive(std::string path, std::string name, std::string& buffer) -> void;
    EXPORTED auto LoadFileFromArchive(std::string path, std::string name) -> std::vector<uint8_t>;
    EXPORTED auto LoadFileFromArchive(std::string path, std::string name, size_t start, size_t count) -> std::vector<uint8_t>;
    EXPORTED auto CreateUUID() -> std::string;
}
