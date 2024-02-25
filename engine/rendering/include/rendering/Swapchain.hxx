#pragma once

#include <SDL2/SDL.h>

namespace kyanite::engine::rendering {
	class Swapchain {
	public:
		Swapchain(SDL_Window* window) : _window(window) {}
		virtual ~Swapchain() = default;
		virtual auto Swap() -> void = 0;
	protected:
		SDL_Window* _window;
	};
}