#include "rendering/opengl/GlVertexBuffer.hxx"

#include <glad/glad.h>

namespace kyanite::engine::rendering::opengl {
	GlVertexBuffer::GlVertexBuffer(const void* data, size_t size) {
		_id = 0;
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	GlVertexBuffer::~GlVertexBuffer() {
		glDeleteBuffers(1, &_id);
	}
}