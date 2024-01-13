#pragma once

#include <filesystem>
#include <string>

namespace io {
    class FileHandle;

    auto Init() -> void;
    [[nodiscard]] auto OpenFile(std::filesystem::path path) -> FileHandle*;
}
