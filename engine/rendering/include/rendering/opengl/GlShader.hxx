#pragma once

#include "../Shader.hxx"

#include <cstdint>

namespace kyanite::engine::rendering::opengl {
	class GlShader: public Shader {
	public:
		uint64_t shaderId;

		GlShader(uint64_t shaderId, ShaderType type): Shader("", type, shaderId), shaderId(shaderId) {}

	};
}