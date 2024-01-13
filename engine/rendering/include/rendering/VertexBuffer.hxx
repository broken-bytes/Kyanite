#pragma once

namespace rendering {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;
		virtual void SetData(const void* data, size_t size) = 0;
	};
}