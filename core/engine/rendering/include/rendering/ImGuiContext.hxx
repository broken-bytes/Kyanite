#pragma once

#include "Context.hxx"
#include "Device.hxx"
#include "RenderBackendType.hxx"

#include <SDL2/SDL.h>
#include <imgui.h>

namespace kyanite::engine::rendering {
	class ImmediateGuiContext : public Context {
	public:
		ImmediateGuiContext(
			const std::shared_ptr<Device>& device,
			SDL_Window* window,
			SDL_GLContext context,
			RenderBackendType backend,
			std::shared_ptr<CommandQueue> queue,
			ImGuiContext* imGuiContext
		);
		~ImmediateGuiContext();
		auto Begin() -> void override;
		auto Finish() -> void override;

	private:
		SDL_Window* _window;
		RenderBackendType _backend;
	};
}