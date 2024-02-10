#pragma once

#include <cstdint>

namespace kyanite::engine::rendering {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;
		virtual auto Id() const->uint64_t = 0;
		virtual void SetData(const void* data, size_t size) = 0;
		virtual void Bind() const = 0;
	};
}