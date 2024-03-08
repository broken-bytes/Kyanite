#pragma once

namespace kyanite::engine::rendering {
	class Buffer {
	public:
		Buffer(size_t size) : _size(size) {}
		virtual ~Buffer() = 0 {}

		auto Size() const -> size_t { return _size; }

	private:
		size_t _size;
	};
}