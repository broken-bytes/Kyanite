#include "rendering/Rendering.hxx"
#include "rendering/Device.hxx"
#include "rendering/opengl/GLDevice.hxx"
#include "rendering/GraphicsContext.hxx"
#include "rendering/ImGuiContext.hxx"
#include "rendering/VertexBuffer.hxx"
#include "rendering/DeviceFactory.hxx"
#include "core/Core.hxx"

#include <FreeImagePlus.h>
#include <glad/glad.h>
#include <SDL.h>
#include <imgui.h>

#include <stdexcept>
#include <map>
#include <memory>
#include <rendering/renderdoc_app.h>

namespace core = kyanite::engine::core;

namespace kyanite::engine::rendering {
	std::shared_ptr<Device> device = nullptr;
	SDL_Window* window = nullptr;

	std::map<uint64_t, std::shared_ptr<VertexBuffer>> vertexBuffers = {};
	std::map<uint64_t, std::shared_ptr<IndexBuffer>> indexBuffers = {};

	std::unique_ptr<GraphicsContext> graphicsContext = nullptr;
	std::unique_ptr<ImGuiContext> imguiContext = nullptr;
	std::unique_ptr<Swapchain> swapchain = nullptr;

	auto Init(NativePointer window) -> void {
		FreeImage_Initialise();
		SDL_InitSubSystem(SDL_INIT_VIDEO);
		auto sdlWindow = reinterpret_cast<SDL_Window*>(window);
		kyanite::engine::rendering::window = sdlWindow;

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
		device = DeviceFactory::CreateDevice(RenderBackendType::OpenGL, sdlWindow);

		graphicsContext = device->CreateGraphicsContext();
		imguiContext = device->CreateImGuiContext();
		swapchain = device->CreateSwapchain();
	}

	auto Shutdown() -> void {
		// Cleanup
		device = nullptr;
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

    auto PreFrame() -> void {
        // ImGui new frame, resource loading, etc.
        // Start the ImGui frame
		imguiContext->Begin();

		// Upload all the resources before rendering
    }

	auto Update(float deltaTime) -> void {
        // Update the game state of the rendering engine
	}

	auto PostFrame() -> void {		
        // Render the actual frame

		imguiContext->End();
		glViewport(0, 0, 800, 600);
		glClearColor(0.15f, 0.2f, 0.3f, 1.0f);
		// First, clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		imguiContext->Draw();

		// Finally, swap the buffers
		swapchain->Swap();
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

	auto CreateVertexBuffer(const void* data, size_t size) -> uint64_t {
		auto buffer = device->CreateVertexBuffer(data, size);
		auto id = buffer->Id();

		vertexBuffers[id] = buffer;
		
		return id;
	}

	auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> uint64_t {
		auto buffer = device->CreateIndexBuffer(indices, len);
		auto id = buffer->Id();

		indexBuffers[id] = buffer;

		return id;
	}

	auto UpdateVertexBuffer(uint64_t buffer, const void* data, size_t size) -> void {

	}

	auto UpdateIndexBuffer(uint64_t buffer, const void* data, size_t size) -> void {

	}

	auto DrawIndexed(uint64_t vertexBuffer, uint64_t indexBuffer, uint64_t material) -> void {
		auto vb = vertexBuffers[vertexBuffer];
		auto ib = indexBuffers[indexBuffer];

		//graphicsContext->DrawIndexed(ib->, 0, 0);
	}
}
