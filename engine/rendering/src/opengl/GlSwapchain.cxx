#include "rendering/opengl/GlSwapchain.hxx"

namespace kyanite::engine::rendering {
	GlSwapchain::GlSwapchain(SDL_Window* window) : Swapchain(window) {

	}

	GlSwapchain::~GlSwapchain() {

	}

	auto GlSwapchain::Swap() -> void {
		SDL_GL_SwapWindow(_window);
	}
}