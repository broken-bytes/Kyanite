#include "Mesh.hxx"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_win32.h>
#if _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#else
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_vulkan.h>
#endif

#include "Engine.hxx"
#include "Core/NativeRef.hxx"

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

struct Instance {
  SDL_Window *Window;
  bool Running;
  const char *Name;
  const char *Version;
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

std::vector<NativeRef *> Textures = {};
std::vector<NativeRef *> Meshes = {};
std::vector<NativeRef *> Shaders = {};
std::vector<NativeRef *> Materials = {};



std::map<uint32_t, Transform> Transforms = {};


std::vector<NativeRef*> TransformModel = {};
NativeRef* TransformTexture;
NativeRef* TransformShader;
NativeRef* TransformMaterial;

constexpr int W = 1920;
constexpr int H = 1080;

auto Tick() -> void {
  auto start = std::chrono::high_resolution_clock::now();
  // Update(frametime);
  std::array<float, 3> pos = {0, 0, 0};
  std::array<float, 3> scale = {1, 1, 1};
  std::array<float, 3> rot = {0, 0, 0};

  auto end = std::chrono::high_resolution_clock::now();
  frametime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();

  ticks++;

  Update(frametime);

  SetCamera(0, 0, -10, 0, 0, 0);

  for (auto& transform: Transforms) {
    MeshDrawInfo info;
    info.Flags = MESH_DRAW_INFO_FLAGS_DRAW_OUTLINE;
    info.OutlineWidth = 2;
    info.OutlineColor[0] = 1;
    info.OutlineColor[1] = 0;
    info.OutlineColor[2] = 1;
    info.OutlineColor[3] = 1;


    DrawMesh(transform.first, Meshes[0], Materials[0], info, transform.second);
  }

      int64_t selectionId = GetMouseOverEntityId(0, 0);
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
  );

  auto ctx = ImGui::CreateContext();
  ImGui::SetCurrentContext(ctx);
  ImGuiIO &io = ImGui::GetIO();
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();
  Init(W, H, (void *)GlobalInstance.Window);
  SetRootDir(argv[1]);


  auto comp =  TestComponent {1, 2 , 3};
  auto compName = "TEST";

  Transforms.insert({56, {{0, 0, 20}, {0, 0, -0, 0}, {0.1f, 0.1f, 0.1f}}});
  auto entity = CreateEntity();

  auto testCompId = RegisterComponent(sizeof(TestComponent), alignof(TestComponent), compName);
  AddComponent(entity, testCompId, sizeof(TestComponent), &comp);

  auto dataPtr =  reinterpret_cast<const TestComponent*> (GetComponent(entity, testCompId));
  auto data = *dataPtr;

  auto colorBufferShader = LoadShaderCPU("shaders/EntityIdBuffer.yaml");
  // Load default resources
  LoadShaderGPU(colorBufferShader);

  auto modelInfo =
      LoadModelCPU("models/stylized_orc_warrior/scene.gltf");

  auto shaderInfo = LoadShaderCPU("shaders/PBRDefault.yaml");
  auto shaderRef =
      LoadShaderGPU(shaderInfo);
  Shaders.push_back(shaderRef);

  for (int x = 0; x < modelInfo.MeshCount; x++) {
    Meshes.push_back(LoadMeshGPU(modelInfo.Meshes[x]));
  }

  auto textureInfo = LoadTextureCPU(
      "models/stylized_orc_warrior/textures/body_baseColor.png");

  auto diffuseTex = LoadTextureGPU(textureInfo);
  Textures.push_back(diffuseTex);
  auto material = LoadMaterialGPU("DefaultPBR", shaderRef);
  Materials.push_back(material);

  textureInfo = LoadTextureCPU(
      "models/stylized_orc_warrior/textures/body_normal.png");
  auto normalTex = LoadTextureGPU(textureInfo);
  Textures.push_back(normalTex);

  SetMaterialTexture(material, "Diffuse", diffuseTex);
  SetMaterialTexture(material, "Normal", normalTex);

  SDL_Event event;

  while (GlobalInstance.Running) {
    Tick();
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
        SetCursorPosition(event.motion.x, event.motion.y);
        io.AddMousePosEvent(event.motion.x, event.motion.y);
        camY = event.motion.xrel;
        camZ = event.motion.yrel;
      default:
        break;
      } // End switch

    } // End while
  }
  SDL_DestroyWindow(GlobalInstance.Window);
  SDL_Quit();

  return 0;
}
