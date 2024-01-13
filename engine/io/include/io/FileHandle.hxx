#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace io {
    class FileHandle {
        public:
            FileHandle(std::filesystem::path path);
            FileHandle(const FileHandle& other) = delete;
            ~FileHandle();
            [[nodiscard]] auto Read() -> std::vector<std::byte>;
            [[nodiscard]] auto Read(uint64_t numBytes) -> std::vector<std::byte>;
            [[nodiscard]] auto ReadString() -> std::string;
            auto Advance(uint64_t bytes) -> void;
            auto Reset() -> void;
            auto Write(std::vector<std::byte> data) -> void;
        private:
            std::filesystem::path _path;
            std::fstream _file;
            uint64_t _position;
    };
}