#pragma once

#include "../Swapchain.hxx"

#include <SDL2/SDL.h>

namespace kyanite::engine::rendering {
	class GlSwapchain : public Swapchain {
	public:
		GlSwapchain(SDL_Window* window);
		~GlSwapchain();
		auto Swap() -> void override;
	};
}