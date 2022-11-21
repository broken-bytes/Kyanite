#if _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#else
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_vulkan.h>
#endif

#include <thread>
#include <filesystem>
#include <memory>
#include <map>
#include <sstream>
#include <regex>
#include <array>


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <dlfcn.h>
#endif


std::vector<uint64_t> ShaderIndices;
std::vector<uint64_t> MeshIndices;
std::vector<uint64_t> TextureIndices;


struct Instance {
	SDL_Window* Window;
	bool Running;
	const char* Name;
	const char* Version;
};


float frametime = 0;
float lastFrametime = 0;

float XInput;
float YInput;
float WInput;
float AInput;


int ticks = 0;


Instance GlobalInstance = {};

auto Tick() -> void
{
	auto start = std::chrono::high_resolution_clock::now();
	//Update(frametime);
	std::array<float, 3> pos = { 0, 0, 0 };
	std::array<float, 3> scale = { 1, 1, 1 };
	std::array<float, 3> rot = { 0, 0, 0 };

	
	auto end = std::chrono::high_resolution_clock::now();
	frametime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	printf("Frametime: %f\n", frametime);

	ticks++;
}


int main(int argc, char* argv[]) {
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	GlobalInstance.Window = SDL_CreateWindow(
		"SDL2Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1024,
		768,
#ifdef __APPLE__
        SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
#else
		SDL_WINDOW_SHOWN
#endif
	);

	SDL_Event event;

	while (!GlobalInstance.Running) {
		Tick();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_WINDOWEVENT:
				break;
			case SDL_USEREVENT:
				//HandleUserEvents(&event);
				break;

			case SDL_KEYDOWN:
				// Handle any key presses here.
				break;

			case SDL_MOUSEBUTTONDOWN:
				// Handle mouse clicks here.
				break;

			case SDL_QUIT:
				GlobalInstance.Running = true;
				break;

			case SDL_MOUSEMOTION:
				XInput = event.motion.x;
				YInput = event.motion.y;
			default:
				break;
			}   // End switch

		}   // End while
	}
	SDL_DestroyWindow(GlobalInstance.Window);
	SDL_Quit();

	return 0;

}

