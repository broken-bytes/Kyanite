#include "io/IOSystem.hxx"
#include "io/FileHandle.hxx"

#include <filesystem>

auto io::Init() -> void {
}

auto io::OpenFile(std::filesystem::path path) -> FileHandle* {
	return new FileHandle(path);
}
