#include "rendering/Rendering.hxx"
#include "rendering/Device.hxx"
#include "rendering/opengl/GLDevice.hxx"
#include "core/Core.hxx"

#include <FreeImagePlus.h>
#include <glad/glad.h>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>
#include <memory>
#include <rendering/renderdoc_app.h>

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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetSwapInterval(0);
		// Create an OpenGL context
		context = SDL_GL_CreateContext(sdlWindow);
		if (!context) {
			auto error = SDL_GetError();
			std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
			throw std::runtime_error("Failed to create OpenGL context");
		}

		SDL_GL_MakeCurrent(sdlWindow, context);

		// Initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			throw std::runtime_error("Failed to initialize GLAD");
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Create ImGui context
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(sdlWindow, context);
		ImGui_ImplOpenGL3_Init("#version 130");

		RENDERDOC_API_1_1_2* rdoc_api = NULL;

		// At init, on windows
		if (HMODULE mod = GetModuleHandleA("renderdoc.dll"))
		{
			pRENDERDOC_GetAPI RENDERDOC_GetAPI =
				(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
			int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&rdoc_api);
			assert(ret == 1);
		}

		// Create a device
		device = std::make_shared<opengl::GlDevice>();
	}

	auto Shutdown() -> void {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		SDL_GL_DeleteContext(context);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

    auto PreFrame() -> void {
        // ImGui new frame, resource loading, etc.
                // Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
    }

	auto Update(float deltaTime) -> void {
        // Update the game state of the rendering engine
	}

	auto PostFrame() -> void {		
        // Render the actual frame

		ImGui::Render();
		glViewport(0, 0, 800, 600);
		glClearColor(0.15f, 0.2f, 0.3f, 1.0f);
		// First, clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	auto LoadShader(std::string code, ShaderType type) -> uint64_t {
		return device->CompileShader(code, type);
	}

	auto UnloadShader(uint64_t shader) -> void {
		device->DestroyShader(shader);
	}

	auto LoadModel(std::string_view path) -> std::vector<Mesh> {
		return std::vector<Mesh>();
	}
}