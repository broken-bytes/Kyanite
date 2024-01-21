#include "rendering/opengl/GlIndexBuffer.hxx"

namespace kyanite::engine::rendering::opengl {
	GlIndexBuffer::GlIndexBuffer(const std::vector<uint32_t>& indices) {
		_id = 0;
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	}

	GlIndexBuffer::~GlIndexBuffer() {
		glDeleteBuffers(1, &_id);
	}
}