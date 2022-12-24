#include <SDL_events.h>
#include <SDL_keycode.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_win32.h>
#include <chrono>
#if _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#else
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_vulkan.h>
#endif

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#if _DEBUG
#include "./renderdoc_app.h"
RENDERDOC_API_1_5_0 *rdoc_api = NULL;
#endif
#endif

#ifdef __APPLE__
#include <dlfcn.h>
#endif

typedef void EditorTick();
typedef void Tick(float);



struct Instance {
  SDL_Window *Window;
  bool Running;
  const char *Name;
  const char *Version;
  EditorTick* EditorTick;
  Tick* Tick;
};

float frametime = 0;
float lastFrametime = 0;

int ticks = 0;
Instance GlobalInstance = {};

constexpr int W = 1920;
constexpr int H = 1080;

auto OnTick() -> void {
  auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  GlobalInstance.Tick(frametime);
  GlobalInstance.EditorTick();
  frametime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  ticks++;
}

int main(int argc, char *argv[]) {
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

#if _WIN32

    auto lib = LoadLibraryA("Kyanite-Editor.dll");
    GlobalInstance.EditorTick = (EditorTick*)GetProcAddress(lib, "editorTick");
    GlobalInstance.Tick = (Tick*)GetProcAddress(lib, "tick");
#if _DEBUG
  if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
    pRENDERDOC_GetAPI RENDERDOC_GetAPI =
        (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
    int ret =
        RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_5_0, (void **)&rdoc_api);
  }
#endif
#endif

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO |
           SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
  SDL_DisplayMode mode = {};
  SDL_GetDesktopDisplayMode(0, &mode);
  GlobalInstance.Window = SDL_CreateWindow("SDL2Test", SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED, W, H,
#ifdef __APPLE__
                                           SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
#else
                                            SDL_WINDOW_SHOWN |
#endif
    SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
  );

  SDL_Event event;

  while (!GlobalInstance.Running) {
    OnTick();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_WINDOWEVENT:
        break;
      case SDL_USEREVENT:
        // HandleUserEvents(&event);
        break;

      case SDL_KEYDOWN:
        
        // Handle any key presses here.
        break;
      case SDL_MOUSEBUTTONUP:
        io.AddMouseButtonEvent(0, 0);
        // Handle mouse clicks here.
        break;
      case SDL_MOUSEBUTTONDOWN:
        io.AddMouseButtonEvent(0, 1);
        // Handle mouse clicks here.
        break;

      case SDL_QUIT:
        GlobalInstance.Running = true;
        break;

      case SDL_MOUSEMOTION:
        io.AddMousePosEvent(event.motion.x, event.motion.y);
      default:
        break;
      } // End switch

    } // End while
  }
  SDL_DestroyWindow(GlobalInstance.Window);
  SDL_Quit();

  return 0;
}
