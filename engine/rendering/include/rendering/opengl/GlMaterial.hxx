#pragma once

#include "../Material.hxx"
#include "GlShader.hxx"

#include <glad/glad.h>

#include <memory>

namespace kyanite::engine::rendering::opengl {
	class GlMaterial : public Material {
	public:
		uint32_t programId;
		uint32_t vaoId;

		GlMaterial(
			std::map<ShaderType, std::shared_ptr<Shader>> shaders,
			std::map<std::string, uint64_t> textures,
			std::map<std::string, float> floats,
			std::map<std::string, uint32_t> ints,
			std::map<std::string, bool> bools,
			std::map<std::string, std::array<float, 2>> vec2s,
			std::map<std::string, std::array<float, 3>> vec3s,
			std::map<std::string, std::array<float, 4>> vec4s
		) : Material(
			shaders,
			textures,
			floats,
			ints,
			bools,
			vec2s,
			vec3s,
			vec4s
		) {
			// Convert the shaders to OpenGL shaders
			auto vertexShader = std::static_pointer_cast<GlShader>(shaders[ShaderType::VERTEX]);
			auto fragmentShader = std::static_pointer_cast<GlShader>(shaders[ShaderType::FRAGMENT]);

			// Create shader program
			GLuint shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader->shaderId);
			glAttachShader(shaderProgram, fragmentShader->shaderId);
			glLinkProgram(shaderProgram);

			// Check linking status
			GLint success;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				GLchar infoLog[512];
				glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
				std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
				// TODO: Throw exception
			}
			programId = shaderProgram;

			// Create VAO
			glGenVertexArrays(1, &vaoId);
			glBindVertexArray(vaoId);
			// Enable the vertex attribute array
			glEnableVertexAttribArray(0);

			// Set up the vertex attribute pointer vec3.
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		}
	};
}