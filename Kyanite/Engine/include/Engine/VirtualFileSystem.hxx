#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace VirtualFileSystem {
	enum class FileType {
		DIRECTORY,
		FILE
	};

	struct FileEntry {
		FileType Type;
		std::string Path;
		FileEntry* Parent;
		std::vector<FileEntry> SubFiles;
	};

	class FileSystem {
	public:
		FileSystem(std::string file);
		auto Open(FileEntry dir)->std::vector<FileEntry>;
		auto Close() -> void;
		auto ReadFile(std::string path) const -> const char*;
		auto WriteFile(std::string path, char* obj, size_t size)->FileEntry;
	private:
		struct FileSystemHeader {
			std::vector<FileEntry> Files;
		};

		std::string _path;
		FileSystemHeader _header;
		auto ParseFileHeader()->FileSystemHeader;
	};
	
	auto Create(std::string path)->std::shared_ptr<FileSystem>;
	auto Mount(std::string file)->std::shared_ptr<FileSystem>;
	auto Unmount(std::shared_ptr<FileSystem> fileSystem) -> void;
}