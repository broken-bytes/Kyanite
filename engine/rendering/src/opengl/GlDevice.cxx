#include "rendering/opengl/GlDevice.hxx"
#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/opengl/GlCommandQueue.hxx"
#include "rendering/opengl/GlFence.hxx"
#include "rendering/Shader.hxx"
#include "rendering/opengl/GlIndexBuffer.hxx"
#include "rendering/opengl/GlVertexBuffer.hxx"
#include "rendering/opengl/GlSwapchain.hxx"
#include "rendering/RenderBackendType.hxx"
#include "rendering/GraphicsContext.hxx"
#include "rendering/ImGuiContext.hxx"

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GL/gl.h>

#include <stdexcept>

namespace kyanite::engine::rendering::opengl {
	GlDevice::GlDevice(SDL_Window* window) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetSwapInterval(0);
		// Create an OpenGL context
		auto context = SDL_GL_CreateContext(window);
		if (!context) {
			auto error = SDL_GetError();
			std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
			throw std::runtime_error("Failed to create OpenGL context");
		}

		SDL_GL_MakeCurrent(window, context);

		// Initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			throw std::runtime_error("Failed to initialize GLAD");
		}

		_window = window;
		_glContext = context;
	}

	GlDevice::~GlDevice() {
		SDL_GL_DeleteContext(_glContext);
	}

	auto GlDevice::Shutdown() -> void {

	}

	auto GlDevice::CreateGraphicsContext() -> std::unique_ptr<GraphicsContext> {
		return std::make_unique<GraphicsContext>(this->shared_from_this());
	}

	auto GlDevice::CreateImGuiContext() -> std::unique_ptr<ImGuiContext> {
		return std::make_unique<ImGuiContext>(
			this->shared_from_this(), 
			_window, 
			_glContext, 
			RenderBackendType::OpenGL
		);
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

	auto GlDevice::CreateSwapchain() -> std::unique_ptr<Swapchain> {
		return std::make_unique<GlSwapchain>(_window);
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

	auto GlDevice::CreateVertexBuffer(const void* data, uint64_t size) -> std::shared_ptr<VertexBuffer> {
		auto buffer = std::make_shared<GlVertexBuffer>();

		return buffer;
	}

	auto GlDevice::UpdateVertexBuffer(std::shared_ptr<VertexBuffer> buffer, const void* data, uint64_t size) -> void {

	}

	auto GlDevice::CreateIndexBuffer(const uint32_t* indices, size_t len) -> std::shared_ptr<IndexBuffer> {
		auto buffer = std::make_shared<GlIndexBuffer>();

		return buffer;
	}

	auto GlDevice::UpdateIndexBuffer(std::shared_ptr<IndexBuffer> buffer, std::vector<uint32_t> indices) -> void {

	}

	auto GlDevice::DestroyShader(uint64_t shaderHandle) -> void {
	}
}