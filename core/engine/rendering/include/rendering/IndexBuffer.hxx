#pragma once

#include "Buffer.hxx"

#include <cstdint>

namespace kyanite::engine::rendering {
	class IndexBuffer: public Buffer {
	public:
		IndexBuffer(size_t size) : Buffer(size) {}
		virtual ~IndexBuffer() = 0 {}
		virtual auto Id() const-> uint64_t = 0;
		virtual void SetData(const void* data, size_t size) = 0;
		virtual void Bind() const = 0;
	};
}