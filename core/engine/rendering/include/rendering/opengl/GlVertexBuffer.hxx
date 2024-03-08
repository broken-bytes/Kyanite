#pragma once

#include "../VertexBuffer.hxx"

#include <glad/glad.h>

#include <cstdint>

namespace kyanite::engine::rendering::opengl {
	class GlVertexBuffer : public VertexBuffer {
	public:
		GlVertexBuffer(const void* data = nullptr, size_t size = 0);

		~GlVertexBuffer();

		auto Id() const -> uint64_t override { return _id; }

		auto SetData(const void* data, size_t size) -> void override {
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		auto Bind() const -> void override {
			glBindBuffer(GL_ARRAY_BUFFER, _id);
		}

	private:
		GLuint _id;
	};
}