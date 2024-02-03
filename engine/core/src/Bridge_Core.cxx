#include "core/Bridge_Core.h"
#include "core/Core.hxx"

#include <shared/NativePointer.hxx>

#include <SDL2/SDL.h>
#include <Windows.h>
#include <iostream>

namespace core = kyanite::engine::core;

void Core_Init() {
	core::InitCore();
}

NativePointer Core_CreateWindow(
	const char* title,
	uint32_t* posX,
	uint32_t* posY,
	uint32_t width,
	uint32_t height,
	uint32_t flags,
	uint8_t renderBackend,
	bool silent
) {
	auto allFlags = flags;
	// Enable OpenGL on the window

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if(renderBackend == 0) {
		allFlags |= SDL_WINDOW_OPENGL;
	} else if(renderBackend == 1) {
		allFlags |= SDL_WINDOW_VULKAN;
	} else if(renderBackend == 2) {
		// TODO: Implement DirectX 12
	} else {
		// throw std::runtime_error("Invalid render backend");
	}

	if (silent) {
		allFlags |= SDL_WINDOW_SKIP_TASKBAR;
		allFlags |= SDL_WINDOW_BORDERLESS;
	}
	auto window = reinterpret_cast<NativePointer>(SDL_CreateWindow(
		title,
		posX != nullptr ? *posX : SDL_WINDOWPOS_UNDEFINED,
		posY != nullptr ? *posY : SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		allFlags
	));

	return window;
}

NativePointer Core_CreateWindowFromNative(NativePointer nativeWindow) {
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_Window* sdlWnd = SDL_CreateWindowFrom(nativeWindow);

	return reinterpret_cast<NativePointer>(sdlWnd);
}

void Core_DestroyWindow(NativePointer window) {
	SDL_DestroyWindow(reinterpret_cast<SDL_Window*>(window));
}

void Core_ShowWindow(NativePointer window) {
	SDL_ShowWindow(reinterpret_cast<SDL_Window*>(window));
}

void Core_HideWindow(NativePointer window) {
	SDL_HideWindow(reinterpret_cast<SDL_Window*>(window));
}
