#pragma once

namespace kyanite::engine::rendering {
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;
		virtual auto Id() const->uint64_t = 0;
		virtual void SetData(const void* data, size_t size) = 0;
		virtual void Bind() const = 0;
	};
}