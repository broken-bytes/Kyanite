#include "io/FileHandle.hxx"

#include <fstream>

namespace io {
    FileHandle::FileHandle(std::filesystem::path path) {
		_path = path;
		_file = std::fstream(path, std::ios::in | std::ios::binary);
		Reset();
    }

	FileHandle::~FileHandle() {
		_file.close();
	}

	auto FileHandle::Read() -> std::vector<std::byte> {
		return Read(UINT64_MAX);
	}

	auto FileHandle::Read(uint64_t numBytes) -> std::vector<std::byte> {
		if (!_file) {
			throw std::runtime_error(_path.string() + ": " + std::strerror(errno));
		}

		auto end = _file.tellg();
		_file.seekg(0, _position);

		auto size = std::size_t(end - _file.tellg());
		size = std::min(size, numBytes);

		_position += size;

		if (size == 0) {
			return {};
		}

		std::vector<std::byte> buffer(size);

		if (!_file.read((char*)buffer.data(), buffer.size())) {
			throw std::runtime_error(_path.string() + ": " + std::strerror(errno));
		}

		return buffer;
	}

	auto FileHandle::ReadString() -> std::string {
		constexpr auto read_size = std::size_t(8192);
		_file.exceptions(std::ios_base::badbit);

		if (not _file) {
			throw std::ios_base::failure("file does not exist");
		}

		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (_file.read(&buf[0], read_size)) {
			out.append(buf, 0, _file.gcount());
		}
		out.append(buf, 0, _file.gcount());
		return out;
	}

	auto FileHandle::Advance(uint64_t bytes) -> void {
        _position += bytes;
	}

	auto FileHandle::Write(std::vector<std::byte> data) -> void {
		_file.write((char*)data.data(), data.size());
	}

	auto FileHandle::Reset() -> void {
		_position = std::ios::beg;
	}
}