#include "rendering/ImGuiContext.hxx"
#include <logger/Logger.hxx>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace kyanite::engine::rendering {
	ImmediateGuiContext::ImmediateGuiContext(
		const std::shared_ptr<Device>& device,
		SDL_Window* window,
		SDL_GLContext context,
		RenderBackendType backend,
		std::shared_ptr<CommandQueue> queue,
		ImGuiContext* imGuiContext
	) : _window(window), Context(CommandListType::Graphics, device, queue) {
		ImGui::SetCurrentContext(imGuiContext);
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init("#version 130");

		_backend = backend;
	}

	auto ImmediateGuiContext::Begin() -> void {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(_window);
		ImGui::NewFrame();
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiWindowFlags window_flags = 0;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
	}

	ImmediateGuiContext::~ImmediateGuiContext() {
		ImGui_ImplSDL2_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	auto ImmediateGuiContext::Finish() -> void {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
	}
}