#include "core/Core.hxx"
#include "core/ILogger.hxx"
#include "core/VendorSerializers/GlmSerializers.hxx"

#include <SDL2/SDL.h>
#include <SDL2/SDL_Vulkan.h>
#include <sodium.h>
#include <zip.h>
#include <stduuid/uuid.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <cassert>

namespace kyanite::engine::core {
	auto InitCore() -> void {
		SDL_Init(SDL_INIT_EVENTS);
	}

	auto LoadFileToBuffer(std::string_view path) -> std::vector<uint8_t> {
		auto file = std::ifstream(std::filesystem::path(path), std::ios::binary);
		auto buffer = std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});

		return buffer;
	}

	auto SaveBufferToFile(std::string_view path, std::vector<uint8_t>& buffer) -> void {
		std::filesystem::create_directories(std::filesystem::path(path).parent_path());
		auto file = std::ofstream(std::filesystem::path(path));
		file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	}

	auto CreateDirectory(std::string_view path) -> void {
		std::filesystem::create_directory(std::filesystem::path(path));
	}

	auto CreateArchive(std::string_view path) -> void {
		zip_t* archive;
		int err;

		if ((archive = zip_open(path.data(), ZIP_CREATE, &err)) == nullptr) {
			zip_error_t error;
			zip_error_init_with_code(&error, err);
			zip_error_fini(&error);

			return;
		}

		zip_close(archive);
	}

	EXPORTED auto OpenArchive(std::string path) -> void {
		zip_t* archive;
		int err;

		if ((archive = zip_open(path.c_str(), ZIP_RDONLY, &err)) == nullptr) {
			zip_error_t error;
			zip_error_init_with_code(&error, err);
			zip_error_fini(&error);

			throw std::runtime_error("Failed to open archive");
		}
	}

	auto CreateFile(std::string path) -> void {
		auto file = std::ofstream(std::filesystem::path(path));
		file.close();
	}

	auto RemoveFile(std::string path) -> void {
		std::filesystem::remove(std::filesystem::path(path));
	}

	auto GetFileList(std::filesystem::path path) -> std::vector<std::filesystem::directory_entry> {
		auto files = std::vector<std::filesystem::directory_entry>();

		for (auto& p : std::filesystem::directory_iterator(path)) {
			files.push_back(p);
		}

		return files;
	}

	auto CheckIfFileExists(std::string path) -> bool {
		return std::filesystem::exists(std::filesystem::path(path));
	}

	auto CheckIfFileExists(std::string archivePath, std::string filePath) -> bool {
		zip_t* archive;
		int err;

		if ((archive = zip_open(archivePath.c_str(), ZIP_RDONLY, &err)) == nullptr) {
			zip_error_t error;
			zip_error_init_with_code(&error, err);
			zip_error_fini(&error);

			return false;
		}

		zip_stat_t stat;
		zip_stat_init(&stat);
		zip_stat(archive, filePath.c_str(), ZIP_FL_ENC_UTF_8, &stat);
		zip_close(archive);

		return stat.valid & ZIP_STAT_SIZE;
	}

    auto SaveBufferToArchive(std::string path, std::string name, std::string& buffer) -> void {
        zip_t* archive;
        int err;

        if ((archive = zip_open(path.c_str(), ZIP_CREATE, &err)) == nullptr) {
            zip_error_t error;
            zip_error_init_with_code(&error, err);
            zip_error_fini(&error);

            return;
        }

        zip_source_t* s = nullptr;
        if ((s = zip_source_buffer(archive, buffer.data(), buffer.size(), 0)) == NULL ||
            zip_file_add(archive, name.c_str(), s, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) < 0) {
            zip_source_free(s);
        }

        zip_close(archive);
    }

    auto LoadFileFromArchive(std::string path, std::string name) -> std::vector<uint8_t> {
		zip_t* archive;
		int err;

		if ((archive = zip_open(path.c_str(), ZIP_RDONLY, &err)) == nullptr) {
			zip_error_t error;
			zip_error_init_with_code(&error, err);
			zip_error_fini(&error);

			return {};
		}

		zip_stat_t stat;
		zip_stat_init(&stat);
		zip_stat(archive, name.c_str(), ZIP_FL_ENC_UTF_8, &stat);
		zip_close(archive);

		return LoadFileFromArchive(path, name, 0, stat.size);
    }

	auto LoadFileFromArchive(std::string path, std::string name, size_t start, size_t count) -> std::vector<uint8_t> {
		zip_t* archive;
		int err;

		if ((archive = zip_open(path.c_str(), ZIP_RDONLY, &err)) == nullptr) {
			zip_error_t error;
			zip_error_init_with_code(&error, err);
			zip_error_fini(&error);

			return {};
		}

		zip_stat_t stat;
		zip_stat_init(&stat);
		zip_stat(archive, name.c_str(), ZIP_FL_ENC_UTF_8, &stat);

		zip_file_t* file = zip_fopen(archive, name.c_str(), ZIP_FL_ENC_UTF_8);
		if (file == nullptr) {
			zip_close(archive);
			return {};
		}

		auto buffer = std::vector<uint8_t>(count);
		auto result = zip_fseek(file, start, SEEK_SET);
		result = zip_file_is_seekable(file);
		result = zip_fread(file, buffer.data(), count);
		result = zip_fclose(file);
		result = zip_close(archive);

		return buffer;
	}

	auto CreateUUID() -> std::string {
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 generator(seq);
		uuids::uuid_random_generator gen{ generator };

		uuids::uuid const id = gen();
		assert(!id.is_nil());
		assert(id.as_bytes().size() == 16);
		assert(id.version() == uuids::uuid_version::random_number_based);
		assert(id.variant() == uuids::uuid_variant::rfc);

		return uuids::to_string(id);
	}
}