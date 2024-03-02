#include "rendering/Rendering.hxx"
#include "rendering/Device.hxx"
#include "rendering/opengl/GLDevice.hxx"
#include "rendering/GraphicsContext.hxx"
#include "rendering/ImGuiContext.hxx"
#include "rendering/UploadContext.hxx"
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

	// Resource management
	std::map<uint64_t, std::shared_ptr<VertexBuffer>> vertexBuffers = {};
	std::map<uint64_t, std::shared_ptr<IndexBuffer>> indexBuffers = {};
	std::map<uint32_t, std::shared_ptr<Texture>> textures = {};
	std::map<uint32_t, std::shared_ptr<Shader>> shaders = {};
	std::map<uint32_t, std::shared_ptr<Material>> materials = {};
	std::map<uint32_t, std::shared_ptr<Mesh>> meshes = {};

	std::unique_ptr<GraphicsContext> graphicsContext = nullptr;
	std::unique_ptr<ImmediateGuiContext> imguiContext = nullptr;
	std::unique_ptr<UploadContext> uploadContext = nullptr;
	std::unique_ptr<Swapchain> swapchain = nullptr;

	std::shared_ptr<Material> testMaterial = nullptr;
	std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> indexBuffer = nullptr;
	std::vector<float> vertices = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	auto Init(NativePointer window, ImGuiContext* context) -> void {
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
		imguiContext = device->CreateImGuiContext(context);
		uploadContext = device->CreateUploadContext();
		swapchain = device->CreateSwapchain();

		vertexBuffer = device->CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
		std::vector<uint32_t> indices = { 0, 1, 2 };
		indexBuffer = device->CreateIndexBuffer(indices.data(), indices.size());

		auto vertexShader = device->CompileShader(R"(
#version 330 core

layout(location = 0) in vec3 aPos;

void main() {
	gl_Position = vec4(aPos, 1.0);
}
		)", ShaderType::VERTEX);
		auto pixelShader = device->CompileShader(R"(
#version 330 core
			
out vec4 fragColor;
	
vec3 hsv_to_rgb(vec3 c) {
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
		
void main() {
	float hue = mod(gl_FragCoord.x / 800.0 + gl_FragCoord.y / 600.0, 1.0);
    fragColor = vec4(hsv_to_rgb(vec3(hue, 1.0, 1.0)), 1.0);
}
		)", ShaderType::FRAGMENT);

		testMaterial = device->CreateMaterial(
			{
				{
					ShaderType::VERTEX, vertexShader
				},
				{
					ShaderType::FRAGMENT, pixelShader
				}
			}
		);
	}

	auto Shutdown() -> void {
		// Cleanup
		device = nullptr;
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	auto PreFrame() -> void {
		// ImGui new frame, resource loading, etc.
		// Start the ImGui frame
		graphicsContext->Begin();
		imguiContext->Begin();
		uploadContext->Begin();
		graphicsContext->ClearRenderTarget();
		graphicsContext->SetViewport(0, 0, 800, 600, 0.0, 1.0);
		graphicsContext->SetScissorRect(0, 0, 800, 600);

		// Upload all the resources before rendering
	}

	auto Update(float deltaTime) -> void {
		vertices[0] = -0.5f + sin(deltaTime);
		uploadContext->UpdateVertexBuffer(vertexBuffer, vertices.data(), vertices.size() * sizeof(float));
		uploadContext->Finish();
		graphicsContext->SetVertexBuffer(0, vertexBuffer);
		graphicsContext->SetIndexBuffer(indexBuffer);
		graphicsContext->SetMaterial(testMaterial);
		graphicsContext->DrawIndexed(indexBuffer->Size(), 0, 0);
		// Update the game state of the rendering engine
	}

	auto PostFrame() -> void {
		// Render the actual frame

		graphicsContext->Finish();
		imguiContext->Finish();

		// Finally, swap the buffers
		swapchain->Swap();
	}

	auto LoadTexture(std::string_view path) -> uint32_t {
		auto buffer = core::LoadFileToBuffer(path);

		FIMEMORY* memory = FreeImage_OpenMemory(buffer.data(), buffer.size());
		FIBITMAP* image = FreeImage_LoadFromMemory(FIF_UNKNOWN, memory);
		FreeImage_CloseMemory(memory);

		if (image == nullptr) {
			throw std::runtime_error("Failed to load image");
		}

		return 0;
	}

	auto LoadShader(
		std::string code,
		ShaderType type
	) -> uint32_t {
		auto shader = device->CompileShader(code, type);
		shaders[shader->id] = shader;

		return shader->id;
	}

	auto UnloadShader(uint64_t shader) -> void {
		device->DestroyShader(shader);
	}

	auto LoadModel(std::string_view path) -> std::vector<Mesh> {
		return std::vector<Mesh>();
	}

	auto CreateVertexBuffer(const void* data, size_t size) -> uint32_t {
		auto buffer = device->CreateVertexBuffer(data, size);
		auto id = buffer->Id();

		vertexBuffers[id] = buffer;

		return id;
	}

	auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> uint32_t {
		auto buffer = device->CreateIndexBuffer(indices, len);
		auto id = buffer->Id();

		indexBuffers[id] = buffer;

		return id;
	}

	auto UpdateVertexBuffer(uint32_t buffer, const void* data, size_t size) -> void {

	}

	auto UpdateIndexBuffer(uint32_t buffer, const void* data, size_t size) -> void {

	}

	auto DrawIndexed(uint32_t vertexBuffer, uint32_t indexBuffer, uint32_t material) -> void {
		auto vb = vertexBuffers[vertexBuffer];
		auto ib = indexBuffers[indexBuffer];

		graphicsContext->SetVertexBuffer(0, vb);
		graphicsContext->SetIndexBuffer(ib);
		graphicsContext->SetMaterial(materials[material]);
		graphicsContext->DrawIndexed(1, 0, 0);
	}
}
