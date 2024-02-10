#pragma once

#include "../IndexBuffer.hxx"

#include <glad/glad.h>

#include <cstdint>
#include <vector>

namespace kyanite::engine::rendering::opengl {
	class GlIndexBuffer : public IndexBuffer {
	public:
		GlIndexBuffer(const std::vector<uint32_t>& data);
		~GlIndexBuffer();

		auto Id() const -> uint64_t { return _id; }

	private:
		GLuint _id;
	};
}