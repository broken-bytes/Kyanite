#include "rendering/ImGuiContext.hxx"

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace kyanite::engine::rendering {
	ImGuiContext::ImGuiContext(
		const std::shared_ptr<Device>& device,
		SDL_Window* window,
		SDL_GLContext context,
		RenderBackendType backend,
		std::shared_ptr<CommandQueue> queue
	) : _window(window), Context(CommandListType::Graphics, device, queue) {
		// Create ImGui context
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init("#version 130");
		
		_backend = backend;
	}

	auto ImGuiContext::Begin() -> void {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(_window);
		ImGui::NewFrame();
	}

	ImGuiContext::~ImGuiContext() {
		ImGui_ImplSDL2_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	auto ImGuiContext::Finish() -> void {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}