#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL2/SDL.h>

#if _WIN32
#include <SDL2/SDL_syswm.h>
#else
#include <SDL_syswm.h>
#include <SDL_vulkan.h>
#endif

#include <iostream>
#include <array>
#include <filesystem>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <thread>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#if _DEBUG
#include "./renderdoc_app.h"
RENDERDOC_API_1_5_0* rdoc_api = NULL;
#endif
#endif

#ifdef __APPLE__
#include <dlfcn.h>
#endif

// ArgsCount, ArgsVec, Width, Height, SDL Window, Root Dir
typedef void RuntimeStart(int argc, char* argv[], uint32_t, uint32_t, void*);
typedef void RuntimeTick();

struct Instance {
	SDL_Window* Window;
	bool Running;
	const char* Name;
	const char* Version;
	RuntimeTick* Tick;
	uint32_t Width;
	uint32_t Height;
};

Instance GlobalInstance = {};

constexpr int W = 1920;
constexpr int H = 1080;


#ifdef _WIN32

void bAttachToConsole()
{
	AllocConsole();
}

#endif

auto Tick() -> void {
	GlobalInstance.Tick();
}

struct TestComponent {
	float x;
	float y;
	float z;
};

int main(int argc, char* argv[]) {
#if _WIN32
#if _DEBUG
	if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
		pRENDERDOC_GetAPI RENDERDOC_GetAPI =
			(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
		int ret =
			RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_5_0, (void**)&rdoc_api);
	}
#endif
#endif

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO |
		SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	SDL_DisplayMode mode = {};
	SDL_GetDesktopDisplayMode(0, &mode);
	GlobalInstance.Running = true;
	GlobalInstance.Window = SDL_CreateWindow("SDL2Test", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, W, H,
#ifdef __APPLE__
		SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
#else
		SDL_WINDOW_SHOWN
#endif
		| SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
	);

#if _WIN32
	bAttachToConsole();
	auto lib = LoadLibraryA("Kyanite-Assembly.dll");
	if (!lib) {
		throw std::runtime_error("Could not load engine library... Exiting");
	}
	else {
		((RuntimeStart*)GetProcAddress(lib, "start"))(argc, argv, W, H, GlobalInstance.Window);
		GlobalInstance.Tick = (RuntimeTick*)GetProcAddress(lib, "update");
	}
#endif
	// Setup Dear ImGui style
	SDL_Event event;
	SetConsoleOutputCP(1251);

	while (GlobalInstance.Running) {
		Tick();
	}
	SDL_DestroyWindow(GlobalInstance.Window);
	SDL_Quit();

	return 0;
}
