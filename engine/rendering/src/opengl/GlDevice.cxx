#include "rendering/opengl/GlDevice.hxx"
#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/opengl/GlCommandQueue.hxx"
#include "rendering/opengl/GlFence.hxx"
#include "rendering/Shader.hxx"

#include <glad/glad.h>
#include <GL/gl.h>

#include <stdexcept>

namespace kyanite::engine::rendering::opengl {
	GlDevice::GlDevice() {

	}

	GlDevice::~GlDevice() {

	}

	auto GlDevice::Shutdown() -> void {

	}

	auto GlDevice::CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> {
		return std::make_shared<GlCommandList>(type);
	}

	auto GlDevice::CreateCommandQueue(CommandListType type) -> std::shared_ptr<CommandQueue> {
		return std::make_shared<GlCommandQueue>(type);
	}

	auto GlDevice::CreateFence() -> std::shared_ptr<Fence> {
		return std::make_shared<GlFence>();
	}

	auto GlDevice::CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> {
		return nullptr;
	}

	auto GlDevice::CreateRenderTarget() -> std::shared_ptr<RenderTarget> {
		return nullptr;
	}

	auto GlDevice::CompileShader(const std::string& shaderSource, ShaderType type) -> uint64_t {
		uint64_t shaderTypeId = 0;
		switch (type) {
		case ShaderType::VERTEX:
			shaderTypeId = GL_VERTEX_SHADER;
			break;
		case ShaderType::FRAGMENT:
			shaderTypeId = GL_FRAGMENT_SHADER;
			break;
		default:
			break;
		}

		const auto shaderCodeGl = reinterpret_cast<const GLchar*>(shaderSource.c_str());

		auto shaderId = glCreateShader(shaderTypeId);
		glShaderSource(shaderId, 1, &shaderCodeGl, nullptr);
		glCompileShader(shaderId);

		// Check for errors
		GLint iTestReturn;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &iTestReturn);
		if (iTestReturn == GL_FALSE) {
			GLchar p_cInfoLog[1024];
			int32_t iErrorLength;
			glGetShaderInfoLog(shaderId, 1024, &iErrorLength, p_cInfoLog);
			glDeleteShader(shaderId);
			
			throw std::runtime_error("Error compiling shader: " + std::string(p_cInfoLog));
		}

		return shaderId;
	}

	auto GlDevice::DestroyShader(uint64_t shaderHandle) -> void {
	}
}