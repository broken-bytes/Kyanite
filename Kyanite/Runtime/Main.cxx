#include <SDL_events.h>
#include <SDL_keycode.h>
#if _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#else
#include <SDL.h>
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
RENDERDOC_API_1_5_0 *rdoc_api = NULL;
#endif
#endif

#ifdef __APPLE__
#include <dlfcn.h>
#endif

std::vector<uint64_t> ShaderIndices = {};
std::vector<uint64_t> MeshIndices = {};
std::vector<uint64_t> TextureIndices = {};

// Width, Height, SDL Window, Root Dir, World Name
typedef void RuntimeStart(uint32_t, uint32_t, void*, const char*, const char*);
typedef void RuntimeTick(float);
typedef void RuntimeKeyUp(uint8_t, const char*);
typedef void RuntimeKeyDown(uint8_t, const char*);
typedef void RuntimeMouseUp(uint8_t);
typedef void RuntimeMouseDown(uint8_t);
typedef void RuntimeMouseMoved(int32_t, int32_t);
typedef void RuntimeViewportResized(uint32_t, uint32_t);

struct Instance {
  SDL_Window *Window;
  bool Running;
  const char *Name;
  const char *Version;
  RuntimeTick* Tick;
  RuntimeKeyUp* KeyUp;
  RuntimeKeyDown* KeyDown;
  RuntimeMouseUp* MouseUp;
  RuntimeMouseDown* MouseDown;
  RuntimeMouseMoved* MouseMoved;
  RuntimeViewportResized* ViewportResized;
  uint32_t Width;
  uint32_t Height;
};

float frametime = 0;
float lastFrametime = 0;

float XInput;
float YInput;
float WInput;
float AInput;

int ticks = 0;


float camX = 0;
float camY = 0;
float camZ = 0;

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
  auto start = std::chrono::high_resolution_clock::now();

  auto end = std::chrono::high_resolution_clock::now();
  frametime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

  ticks++;

  GlobalInstance.Tick(frametime);
}

struct TestComponent {
  float x;
  float y;
  float z;
};

int main(int argc, char *argv[]) {
#if _WIN32
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

  //Init(W, H, (void *)GlobalInstance.Window);
  //SetRootDir(argv[1]);
#if _WIN32
    bAttachToConsole();
    auto lib = LoadLibraryA("Kyanite.dll");
    ((RuntimeStart*)GetProcAddress(lib, "start"))(W, H, GlobalInstance.Window, argv[1], "WORLD");
    GlobalInstance.Tick = (RuntimeTick*)GetProcAddress(lib, "update");
    GlobalInstance.KeyUp = (RuntimeKeyUp*)GetProcAddress(lib, "onKeyUp");
    GlobalInstance.KeyDown = (RuntimeKeyDown*)GetProcAddress(lib, "onKeyDown");
    GlobalInstance.MouseUp = (RuntimeMouseUp*)GetProcAddress(lib, "onMouseButtonUp");
    GlobalInstance.MouseDown = (RuntimeMouseDown*)GetProcAddress(lib, "onMouseButtonDown");
    GlobalInstance.MouseMoved = (RuntimeMouseMoved*)GetProcAddress(lib, "onMouseMoved");
    GlobalInstance.ViewportResized = (RuntimeViewportResized*)GetProcAddress(lib, "onViewportResized");
#endif
  // Setup Dear ImGui style
  SDL_Event event;
  SetConsoleOutputCP(1251); 

  while (GlobalInstance.Running) {
    Tick();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
              int width = event.window.data1;
              int height = event.window.data2;

              if (width != GlobalInstance.Width || height != GlobalInstance.Height) {
                  GlobalInstance.Width = width;
                  GlobalInstance.Height = height;

                  GlobalInstance.ViewportResized(width, height);
              }
          }
        break;
      case SDL_USEREVENT:
        // HandleUserEvents(&event);
        break;

      case SDL_KEYUP:
        GlobalInstance.KeyUp(event.key.keysym.scancode, SDL_GetKeyName(event.key.keysym.sym));      
        break;
      case SDL_KEYDOWN:
      GlobalInstance.KeyDown(event.key.keysym.scancode, SDL_GetKeyName(event.key.keysym.sym));      
        break;
      case SDL_MOUSEBUTTONUP:
        GlobalInstance.MouseUp(event.button.button);
        //io.AddMouseButtonEvent(0, 0);
        // Handle mouse clicks here.
        break;
      case SDL_MOUSEBUTTONDOWN:
        GlobalInstance.MouseDown(event.button.button);
        //io.AddMouseButtonEvent(0, 1);
        // Handle mouse clicks here.
        break;

      case SDL_QUIT:
        GlobalInstance.Running = false;
        break;

      case SDL_MOUSEMOTION:
        //io.AddMousePosEvent(event.motion.x, event.motion.y);
        camY = event.motion.xrel;
        camZ = event.motion.yrel;
        GlobalInstance.MouseMoved(event.motion.x, event.motion.y);
        
      default:
        break;
      } // End switch

    } // End while
  }
  SDL_DestroyWindow(GlobalInstance.Window);
  SDL_Quit();

  return 0;
}
