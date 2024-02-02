#include "rendering/Rendering.hxx"
#include "rendering/Device.hxx"
#include "core/Core.hxx"

#include <FreeImagePlus.h>
#include <glad/glad.h>
#include <SDL.h>

#include <stdexcept>
#include <memory>

namespace core = kyanite::engine::core;

namespace kyanite::engine::rendering {
	std::shared_ptr<Device> device = nullptr;
	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	auto Init(NativePointer window) -> void {
		FreeImage_Initialise();
		SDL_InitSubSystem(SDL_INIT_VIDEO);
		auto sdlWindow = reinterpret_cast<SDL_Window*>(window);
		kyanite::engine::rendering::window = sdlWindow;

		// Create an OpenGL context
		context = SDL_GL_CreateContext(sdlWindow);
		if (!context) {
			auto error = SDL_GetError();
			std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
			throw std::runtime_error("Failed to create OpenGL context");
		}

		// Initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			throw std::runtime_error("Failed to initialize GLAD");
		}

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	}

	auto Update(float deltaTime) -> void {

	}

	auto Render() -> void {
		std::cout << "Rendering" << std::endl;
		glViewport(0, 0, 800, 600);
		glClearColor(0.15f, 0.2f, 0.3f, 1.0f);
		// First, clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
		// Then, render the scene

		// Finally, swap the buffers
		SDL_GL_SwapWindow(window);
	}

	auto LoadTexture(std::string_view path) -> Texture {
		auto buffer = core::LoadFileToBuffer(path);

		FIMEMORY* memory = FreeImage_OpenMemory(buffer.data(), buffer.size());
		FIBITMAP* image = FreeImage_LoadFromMemory(FIF_UNKNOWN, memory);
		FreeImage_CloseMemory(memory);

		if (image) {
		}
		else {
			throw std::runtime_error("Failed to load image");
		}

		return Texture();
	}

	auto LoadShader(std::string_view vertexPath, std::string_view fragmentPath) -> Shader {
		return Shader();
	}

	auto LoadModel(std::string_view path) -> std::vector<Mesh> {
		return std::vector<Mesh>();
	}
}