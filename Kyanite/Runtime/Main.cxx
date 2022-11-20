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
#if _DEBUG
#include <renderdoc_app.h>
RENDERDOC_API_1_5_0* rdoc_api = NULL;
#endif
#endif

#ifdef __APPLE__
#include <dlfcn.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#endif

constexpr char NAME[] = "Kyanite";
constexpr char VERSION[] = "0.01a";

bool rendererInitialized = false;
bool done = false;

typedef void* (*EngineInit)(uint32_t, uint32_t, void*, uint8_t, int*);
typedef void* (*RendererInit)(uint32_t, uint32_t, void*, uint8_t);
typedef void* (*VoidEngineFunc)();
typedef void* (*EngineUpdate)(float);
typedef void* (*EngineLoadMap)(const char* name, size_t nameLen);

#define GL_SILENCE_DEPRECATION TRUE


std::vector<uint64_t> ShaderIndices;
std::vector<uint64_t> MeshIndices;
std::vector<uint64_t> TextureIndices;

SDL_Window* Window;
EngineUpdate Update;
EngineLoadMap LoadMap;
VoidEngineFunc ExecuteJobs;
float frametime = 0;

float lastFrametime = 0;
int counter = 0;

float XInput;
float YInput;
float WInput;
float AInput;


TextureInfo tex1;
TextureInfo tex2;
ModelInfo model;


int ticks = 0;

auto Tick() -> void
{
	auto start = std::chrono::high_resolution_clock::now();
	//Update(frametime);
	std::array<float, 3> pos = { 0, 0, 0 };
	std::array<float, 3> scale = { 1, 1, 1 };
	std::array<float, 3> rot = { 0, 0, 0 };

	for (int x = 0; x < MeshIndices.size(); x++) {
		Renderer_DrawMesh(MeshIndices[x], ShaderIndices[0], TextureIndices[0], pos.data(), scale.data(), rot.data());
	}

	Renderer_Update();
	Renderer_RotateCamera(YInput * frametime, XInput * frametime, 0);
	Renderer_MoveCamera(AInput * frametime, 0, WInput * frametime);
	XInput = 0;
	YInput - 0;
	WInput = 0;
	AInput = 0;

#ifdef __APPLE__
	SDL_GL_SwapWindow(Window);
	SDL_GL_SetSwapInterval(2);
#endif
	auto end = std::chrono::high_resolution_clock::now();
	frametime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	printf("Frametime: %f\n", frametime);

	ticks++;

	if (ticks == 20) {
		Engine_FreeModel(model);
		Engine_FreeTexture(tex1);
		Engine_FreeTexture(tex2);
	}
}


#ifdef __APPLE__
void Setup() {
}
#endif

#ifdef _WIN32

#endif


int main(int argc, char* argv[]) {
#if _WIN32
	if (HMODULE mod = GetModuleHandleA("renderdoc.dll"))
	{
		pRENDERDOC_GetAPI RENDERDOC_GetAPI =
			(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
		int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_5_0, (void**)&rdoc_api);
	}
#endif
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	Window = SDL_CreateWindow(
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

	Setup();

	SDL_Event event;

	while (!done) {
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
				done = true;
				break;

			case SDL_MOUSEMOTION:
				XInput = event.motion.x;
				YInput = event.motion.y;
			default:
				break;
			}   // End switch

		}   // End while
	}
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;

}

