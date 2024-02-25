#pragma once

#include "Context.hxx"
#include "Device.hxx"
#include "RenderBackendType.hxx"

#include <SDL2/SDL.h>
#include <imgui.h>

namespace kyanite::engine::rendering {
	class ImGuiContext : public Context {
	public:
		ImGuiContext(
			const std::shared_ptr<Device>& device,
			SDL_Window* window,
			SDL_GLContext context,
			RenderBackendType backend
		);
		~ImGuiContext();
		auto Begin() -> void;
		auto End() -> void;
		auto Draw() -> void;

	private:
		SDL_Window* _window;
		RenderBackendType _backend;
	};
}