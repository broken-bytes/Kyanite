#include "VirtualFileSystem.hxx"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

namespace VirtualFileSystem {

	constexpr char HEADER_START[] = "=== BEGIN ASSET BUNDLE HEADER ===";
	constexpr char HEADER_END[] = "=== END ASSET BUNDLE HEADER ===";

	FileSystem::FileSystem(std::string file) {
		_path = file;
		_header = ParseFileHeader();
	}

	auto FileSystem::Open(FileEntry dir)->std::vector<FileEntry> {
		return {};
	}

	auto FileSystem::Close() -> void {

	}

	auto FileSystem::ReadFile(std::string path) const -> const char* {
		std::ifstream inFile;

		// Opening file in input mode
		inFile.open(_path, std::ios::in);

		// Object of class contestant to input data in file
		char* obj;

		return nullptr;
	}

	auto FileSystem::WriteFile(std::string path, char* obj, size_t size)->FileEntry {
		std::ifstream inFile(_path);

		std::vector<std::string> lines = { };
		std::string line;

		bool hasValidHeader = false;

		while (std::getline(inFile, line)) {
			lines.emplace_back(line);
		}

		auto itemIndex = std::find_if(lines.begin(), lines.end(), [](std::string line) {
			return line == HEADER_END;
		});

		if (itemIndex != lines.end()) {
			auto items = itemIndex - lines.begin();
			std::stringstream lineStr;
			lineStr << path << " " << size;
			auto si = strlen(lineStr.str().c_str());
			lines.emplace(itemIndex, lineStr.str());
		}
		
		inFile.close();

		std::ofstream outFile(_path);

		for (auto& line : lines) {
			outFile << line << "\n";
		}

		outFile.close();
		outFile.open(_path, std::ios::app | std::ios::out | std::ios::binary);
		outFile.write(obj, size);

		outFile.close();


		return {};
	}

	auto FileSystem::ParseFileHeader() -> FileSystemHeader
	{
		std::fstream file;
		file.open(_path);

		if (!file.is_open()) {
			perror("Error open");
			exit(EXIT_FAILURE);
		}
		std::vector<std::string> lines = { };
		std::string line;

		bool hasValidHeader = false;

		while (std::getline(file, line)) {
			lines.emplace_back(line);
			if (line == HEADER_END) {
				break;
			}
		}

		if (lines[0] == HEADER_START && lines[lines.size() - 1] == HEADER_END) {
			
		}
		else {
			throw std::runtime_error("No valid header found.");
		}

		return FileSystemHeader();
	}

	auto Create(std::string path) -> std::shared_ptr<FileSystem>
	{
		std::ofstream file;
		file.open(path);

		file << HEADER_START << "\n" << HEADER_END;
		file.close();

		return std::make_shared<FileSystem>(path);
	}

	auto Mount(std::string file)->std::shared_ptr<FileSystem> {
		return std::make_shared<FileSystem>(file);
	}

	auto Unmount(std::shared_ptr<FileSystem> fileSystem) -> void {

	}

}
