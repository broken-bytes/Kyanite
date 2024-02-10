#pragma once

#include "../VertexBuffer.hxx"

#include <glad/glad.h>

#include <cstdint>

namespace kyanite::engine::rendering::opengl {
	class GlVertexBuffer : public VertexBuffer {
		public:
		GlVertexBuffer(const void* data, size_t size);
		~GlVertexBuffer();

		auto Id() const -> uint64_t { return _id; }

	private:
		GLuint _id;
	};
}