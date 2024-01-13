#pragma once

namespace rendering {
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;
		virtual void SetData(const void* data, size_t size) = 0;
	};
}