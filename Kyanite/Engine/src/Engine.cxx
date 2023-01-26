#include "Engine.hxx"
#include "Core/AssetLoader.hxx"
#include "Mesh.hxx"
#include "Rendering/Interface.hxx"
#include "Rendering/Vertex.hxx"
#include "ResourceTracker.hxx"
#include "Shader.hxx"
#include <SDL2/SDL.h>
#include <algorithm>
#include <memory>
#include <stduuid/uuid.h>
#include <string.h>
#include <thread>
#include <vector>

#include "glm/common.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/common.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/mat4x4.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "glm/vec4.hpp"

#include <imgui.h>

#include "ECSBridge.h"
#include <iostream>
#include <InputHandler.hxx>
#include <SDL2/SDL_syswm.h>

struct EngineInstance {
	std::unique_ptr<Renderer::Interface> Renderer;
	ecs_entity_t Scene;
	ImGuiIO IO;
	ImGuiContext* CTX;
	ImFont* BaseText;
	std::vector<uint32_t> WindowSize = { 0, 0 };
};

EngineInstance Instance = {};


void HandleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				int width = event.window.data1;
				int height = event.window.data2;

				if (width != Instance.WindowSize[0] || height != Instance.WindowSize[1]) {
					Instance.WindowSize[0] = width;
					Instance.WindowSize[1] = height;

					Instance.Renderer->Resize(width, height);
				}
			}
			break;
		case SDL_USEREVENT:
			break;
		case SDL_KEYUP:
			InputHandler::AddKeyboardEvent(event.key.keysym.scancode, InputHandler::ButtonState::Up);
			break;
		case SDL_KEYDOWN:
			InputHandler::AddKeyboardEvent(event.key.keysym.scancode, InputHandler::ButtonState::Down);
			break;
		case SDL_MOUSEBUTTONUP:
			//GlobalInstance.MouseUp(event.button.button);
			InputHandler::AddMouseClickEvent(InputHandler::MouseButton(event.button.button), InputHandler::ButtonState::Up);
			break;
		case SDL_MOUSEBUTTONDOWN:
			InputHandler::AddMouseClickEvent(InputHandler::MouseButton(event.button.button), InputHandler::ButtonState::Down);
			break;

		case SDL_MOUSEMOTION:
			InputHandler::AddMouseMoveEvent(event.motion.x, event.motion.y);
			break;

		case SDL_QUIT:
			exit(0);
			break;
		default:
			break;
		} // End switch
	} // End while

}

void RendererSystem(ecs_iter_t* iterator) {
	size_t numItems = 0;
	auto transforms = reinterpret_cast<TransformComponent*>(ECS_GetComponentData(iterator, sizeof(TransformComponent), 1, &numItems));
	auto meshes = reinterpret_cast<MeshComponent*>(ECS_GetComponentData(iterator, sizeof(MeshComponent), 2, &numItems));
	auto material = reinterpret_cast<MaterialComponent*>(ECS_GetComponentData(iterator, sizeof(MaterialComponent), 3, &numItems));

	for (int x = 0; x < numItems; x++) {
		Renderer_DrawMesh(
			x,
			meshes[x].internalId,
			material[x].internalId,
			MeshDrawInfo{},
			{
				{ transforms[x].Position.x, transforms[x].Position.y, transforms[x].Position.z },
				{ transforms[x].Rotation.x, transforms[x].Rotation.y, transforms[x].Rotation.z },
				{ transforms[x].Scale.x, transforms[x].Scale.y, transforms[x].Scale.z }
			});
	}
}

void SetupEntityRenderer() {
	std::vector<uint64_t> componentIds = {};
	ECS_COMPONENT(Flecs_GetWorld(), TransformComponent);
	ECS_COMPONENT(Flecs_GetWorld(), MeshComponent);
	ECS_COMPONENT(Flecs_GetWorld(), MaterialComponent);
	componentIds.push_back(ecs_id(TransformComponent));
	componentIds.push_back(ecs_id(MeshComponent));
	componentIds.push_back(ecs_id(MaterialComponent));
	ECS_RegisterSystem("Renderer", RendererSystem, false , componentIds.data(), componentIds.size());
}

void SetupBuiltinSystems() {
	SetupEntityRenderer();
}

void SetupImGui() {
	Instance.CTX = ImGui::CreateContext();
	ImGui::SetCurrentContext(Instance.CTX);
	auto io = ImGui::GetIO();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//Instance.BaseText = io.Fonts->AddFontFromFileTTF("H:\\Projects\\Kyanite\\Resources\\fonts\\Metrophobic-Regular.ttf", 24.0f);

	// Enable Gamepad Controls
	ImGui::StyleColorsDark();
}

// NOTE - We are using void* Pointers instead of
// std::shared_ptr<TrackedResource> for the C-style exported API.
//  Reasons:
//  - Swift can only handle C-style types(for now)
//  - We cannot export C++ types from within a C API
//  - C++ name mangling is a mess
//  The void* pointer is still std::shared_ptr<TrackedResource> but cast to
//  void* so Swift can keep a ref to it.

#pragma region ENGINE_API
void Engine_Init(uint32_t resolutionX, uint32_t resolutionY, void* window) {
	SetupImGui();
	Instance.Renderer = std::make_unique<Renderer::Interface>(
		resolutionX, resolutionY, window, Renderer::RenderBackendAPI::DirectX12);

	Flecs_Init(std::thread::hardware_concurrency());

	SetupBuiltinSystems();

	InputHandler::Init();
}

void Engine_Shutdown() {
}

void Engine_Update(float frameTime) {
	HandleEvents();
	Flecs_Update(frameTime);
	Engine_StartRender();
	Engine_EndRender();
	InputHandler::Flush();
}

void Engine_StartRender() {
	Instance.Renderer->StartFrame();
}

void Engine_EndRender() {
	Instance.Renderer->MidFrame();
	Instance.Renderer->EndFrame();
}


void Engine_Resize(uint32_t width, uint32_t height) {
	Instance.Renderer->Resize(width, height);
}

uint64_t Engine_GetOutputTexture() {
	return Instance.Renderer->GetOutputTexture();
}

void Engine_LoadMeshes(const char* path, ModelIdInfo* info) {
	auto model = Engine_LoadModelCPU(path);

	auto data = new uint64_t[model.MeshCount];

	for (int x = 0; x < model.MeshCount; x++) {
		data[x] = Engine_LoadMeshGPU(model.Meshes[x]);
	}

	info->Ids = data;
	info->NumIds = model.MeshCount;
}

uint64_t Engine_LoadShader(const char* path) {
	auto info = Engine_LoadShaderCPU(path);
	return Engine_LoadShaderGPU(info);
}

uint64_t Engine_LoadTexture(const char* path) {
	auto info = Engine_LoadTextureCPU(path);
	return Engine_LoadTextureGPU(info);
}

void Engine_SetMaxFrameRate(uint16_t maxFramerate) {}

void Engine_SetVSync(bool enabled) {}

void Engine_SetRootDir(const char* path) {
	AssetLoader::SetRootDir(path);
}

void Engine_SetCursorPosition(uint32_t x, uint32_t y) {}

void Engine_SetCamera(float xPos, float yPos, float zPos, float xRotation,
	float yRotation, float zRotation) {
	Instance.Renderer->SetCamera({ xPos, yPos, zPos },
		{ xRotation, yRotation, zRotation });
}
#pragma endregion


#pragma region INPUT_API
uint8_t Input_GetKeyboardButton(uint16_t code) {
	return (uint8_t)InputHandler::GetKeyboardButtonState(SDL_Scancode(code));
}

uint8_t Input_GetMouseButton(uint8_t button) {
	return (uint8_t)InputHandler::GetMouseButtonState(InputHandler::MouseButton(button));
}

DLL_EXPORT void Input_GetMouseMovement(float* x, float_t* y) {
	auto movement = InputHandler::GetMouseMovement();
	*x = (float)movement[0] / Instance.WindowSize[0];
	*y = (float)movement[1] / Instance.WindowSize[1];
}

DLL_EXPORT void Input_GetMousePosition(uint32_t* x, uint32_t* y) {
	auto pos = InputHandler::GetMouseAbs();
	*x = pos[0];
	*y = pos[1];
}
#pragma endregion

#pragma region RENDER_API
void Renderer_DrawMesh(uint64_t entityId, uint64_t mesh, uint64_t material,
	MeshDrawInfo info, Transform transform) {
	glm::vec3 position = glm::vec3(transform.Position.X, transform.Position.Y,
		transform.Position.Z);
	glm::vec3 scale =
		glm::vec3(transform.Scale.X, transform.Scale.Y, transform.Scale.Z);
	glm::quat qRotation = glm::quat(transform.Rotation.Q, transform.Rotation.X,
		transform.Rotation.Y, transform.Rotation.Z);
	Instance.Renderer->DrawMesh(entityId, mesh, material, info, position,
		qRotation, scale);
}

void Renderer_DrawLine(float* from, float* to, float* color) {
	Instance.Renderer->DrawLine(*(reinterpret_cast<glm::vec3*>(from)), *(reinterpret_cast<glm::vec3*>(to)), *(reinterpret_cast<glm::vec4*>(color)));
}
#pragma endregion

#pragma region SHADER_API
void Shader_SetMaterialTexture(uint64_t material, const char* name, uint64_t texture) {
	Instance.Renderer->SetMaterialTexture(material, name, texture);
}

void Shader_SetMaterialPropertyInt(uint64_t material, const char* name, int value) {}

void Shader_SetMaterialPropertyFloat(uint64_t material, const char* name,
	float value) {}

void Shader_SetMaterialPropertyVector2(uint64_t material, const char* name,
	float* value) {}

void Shader_SetMaterialPropertyVector3(uint64_t material, const char* name,
	float* value) {}

void Shader_SetMaterialPropertyVector4(uint64_t material, const char* name,
	float* value) {}

#pragma endregion

#pragma region ENTITY_API
uint64_t ECS_CreateEntity(const char* name) {
	return Flecs_CreateEntity(name);
}

void ECS_DeleteEntity(uint64_t id) {
	Flecs_DeleteEntity(id);
}

uint64_t ECS_RegisterComponent(uint64_t size, uint8_t alignment, const char* uuid) {
	return Flecs_RegisterComponent(size, alignment, uuid);
}

uint64_t ECS_AddComponent(uint64_t entity, uint64_t id, uint64_t size, void* data) {
	return Flecs_AddComponent(entity, id, size, data);
}

const void* ECS_GetComponent(uint64_t entity, uint64_t id) {
	return Flecs_GetComponent(entity, id);
}

uint32_t ECS_GetMouseOverEntityId(uint32_t x, uint32_t y) {
	return Instance.Renderer->ReadMouseOverData(x, y);
}

uint64_t ECS_RegisterSystem(const char* name, void* system, bool multiThreaded, uint64_t* componentIds,
	size_t numComponents) {
	return Flecs_RegisterSystem(name, system, multiThreaded, componentIds, numComponents);
}

void* ECS_GetComponentData(void* iterator, size_t size, uint8_t index, size_t* count) {
	return Flecs_GetComponentData(iterator, size, index, count);
}

void ECS_GetSystemDelta(void* iterator, float* delta) {
	Flecs_GetSystemDeltaTime(iterator, delta);
}

uint64_t ECS_GetSystemFromIter(void* iterator) {
	return Flecs_GetSystemFromIter(iterator);
}

#pragma endregion

#pragma region IMGUI_API
void IMGUI_NotifyMouseDown(uint8_t button) {
	ImGui::GetIO().AddMouseButtonEvent(button, 0);
	printf("Mouse button pressed: %i\n", button);
}

void IMGUI_NotifyMouseUp(uint8_t button) {
	ImGui::GetIO().AddMouseButtonEvent(button, 1);
	printf("Mouse button released: %i\n", button);

}

void IMGUI_NotifyMouseMove(int32_t x, int32_t y) {
	ImGui::GetIO().AddMousePosEvent(x, y);
}

void IMGUI_StartWindow(const char* name) {
	ImGui::Begin(name);
}

void IMGUI_EndWindow() {
	ImGui::End();
}

void IMGUI_StartChild() {

}

void IMGUI_EndChild() {

}

void IMGUI_DrawText(const char* text) {
	ImGui::Text(text);
}

void IMGUI_DrawFloatField(const char* title, float* value) {
	ImGui::InputFloat(title, value);
}

void IMGUI_DrawFloat2Field(const char* title, float* value) {

}

void IMGUI_DrawFloat3Field(const char* title, float* value) {

}

void IMGUI_DrawIntField(const char* title, int* value) {
	ImGui::InputInt(title, value);
}

void IMGUI_DrawColorPicker(const char* title, float* color) {

}

#pragma endregion

#pragma region INTERNAL_API
// --- Load Functions ---
// Loads a mesh directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU if
// not supported)
uint64_t Engine_LoadMeshGPU(MeshInfo& info) {
	auto index = Instance.Renderer->UploadMeshData(
		(Vertex*)info.Vertices,
		info.VerticesCount / 13, // 13 Because 13 Float values. Vec4 Pos, Vec3 Normal, Vec2 UV, Vec4 COl 
		info.Indices,
		info.IndicesCount
	);
	return index;
}

// Loads a mesh into CPU memory (RAM)
ModelInfo Engine_LoadModelCPU(const char* path) {
	auto model = AssetLoader::LoadModel(path);

	ModelInfo info;

	MeshInfo* m = new MeshInfo[model.Meshes.size()];
	for (int x = 0; x < model.Meshes.size(); x++) {
		MeshInfo mesh = {};
		mesh.Vertices = new float[model.Meshes[x].Vertices.size()];
		for (int y = 0; y < model.Meshes[x].Vertices.size(); y++) {
			mesh.Vertices[y] = model.Meshes[x].Vertices[y];
		}
		mesh.VerticesCount = model.Meshes[x].Vertices.size();

		mesh.Indices = new uint32_t[model.Meshes[x].Indicies.size()];
		for (int y = 0; y < model.Meshes[x].Indicies.size(); y++) {
			mesh.Indices[y] = model.Meshes[x].Indicies[y];
		}
		mesh.IndicesCount = model.Meshes[x].Indicies.size();
		m[x] = mesh;
	}
	info.Meshes = m;
	info.MeshCount = model.Meshes.size();

	return info;
}

void Engine_FreeModelCPU(ModelInfo& info) { delete[] info.Meshes; }

// Loads a texture directly into the GPU (DxStorage, MetalIO, or via CPU -> GPU
// if not supported)
uint64_t Engine_LoadTextureGPU(TextureInfo& info) {
	return Instance.Renderer->UploadTextureData(info.Levels[0].Data, info.Levels[0].Width,
		info.Levels[0].Height, 4);
}

// Loads a texture into CPU memory (RAM)
TextureInfo Engine_LoadTextureCPU(const char* path) {
	auto texture = AssetLoader::LoadTexture(path);

	TextureInfo info;
	info.LevelCount = texture.Levels.size();
	info.Levels = new TextureLevelInfo[info.LevelCount];
	info.Channels = texture.Channels;
	for (int x = 0; x < info.LevelCount; x++) {
		// Always assume 4 bytes per pixel for now
		size_t bytes = texture.Levels[x].Width * texture.Levels[x].Height * 4;
		info.Levels[x].Data = texture.Levels[x].Data;
		info.Levels[x].Width = texture.Levels[x].Width;
		info.Levels[x].Height = texture.Levels[x].Height;
	}

	return info;
}

void Engine_FreeTextureCPU(TextureInfo& info) {
	for (int x = 0; x < info.LevelCount; x++) {
		delete[] info.Levels[x].Data;
	}

	delete[] info.Levels;
}

ShaderInfo Engine_LoadShaderCPU(const char* path) {
	const char* Name;
	ShaderJSONDataLightingModel Lighting;
	ShaderJSONDataInputProp* Input;
	size_t InputLen;
	const char* Code;
	ShaderInfo info;
	auto shader = AssetLoader::LoadShader(path);

	// Copy code as we cannot pass strings to Swift runtime, so we copy it into
	// char buff
	auto codeLen = shader.Code.size();
	auto code = new char[codeLen + 1];
	shader.Code.copy(code, codeLen);
	code[codeLen] = '\0';
	info.Data.Code = code;
	info.Data.InstancingEnabled = shader.Description.InstancingEnabled;

	// Only default lighting for now
	info.Data.Lighting = shader.Description.IsLit
		? ShaderJSONDataLightingModel::DEFAULT
		: ShaderJSONDataLightingModel::UNLIT;

	switch (shader.Description.Format) {
	case AssetLoader::ShaderAssetOutputFormat::RGBA_FLOAT:
		info.Data.Format = SHADER_OUTPUT_RGBA_FLOAT;
		break;
	case AssetLoader::ShaderAssetOutputFormat::RGBA_UINT:
		info.Data.Format = SHADER_OUTPUT_RGBA_UINT;
		break;
	}

	info.Data.Constants =
		new ShaderJSONDataInputProp[shader.Description.Constants.size()];
	info.Data.ConstantBufferLayout =
		new ShaderJSONDataInputProp[shader.Description.ConstantBufferLayout
		.size()];

	for (auto [it, end, x] = std::tuple{ shader.Description.Constants.begin(),
										shader.Description.Constants.end(), 0 };
		it != end; it++, x++) {
		auto item = it;
		ShaderJSONDataInputPropType type = [&, item]() {
			switch (item->Type) {
			case AssetLoader::ShaderAssetDescriptionPropType::TEXTURE:
				return ShaderJSONDataInputPropType::SHADER_PROP_TEXTURE;
			case AssetLoader::ShaderAssetDescriptionPropType::FLOAT:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT;
			case AssetLoader::ShaderAssetDescriptionPropType::VECTOR2:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT2;
			case AssetLoader::ShaderAssetDescriptionPropType::VECTOR3:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT3;
			case AssetLoader::ShaderAssetDescriptionPropType::VECTOR4:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT4;
			case AssetLoader::ShaderAssetDescriptionPropType::BOOL:
				return ShaderJSONDataInputPropType::SHADER_PROP_BOOL;
			case AssetLoader::ShaderAssetDescriptionPropType::INT:
				return ShaderJSONDataInputPropType::SHADER_PROP_INT;
			default:
				throw std::runtime_error("Unknown shader prop type");
			}
		}();
		ShaderJSONDataInputProp prop = {};
		size_t propNameLen = item->Name.length();
		char* propName = new char[propNameLen + 1];
		item->Name.copy(propName, propNameLen);
		propName[propNameLen] = '\0';
		prop.Name = propName;
		prop.Type = type;
		prop.Slot = item->Slot;
		info.Data.Constants[x] = prop;
	}

	for (auto [it, end, x] =
		std::tuple{ shader.Description.ConstantBufferLayout.begin(),
				   shader.Description.ConstantBufferLayout.end(), 0 };
		it != end; it++, x++) {
		auto item = it;
		ShaderJSONDataInputPropType type = [&, item]() {
			switch (item->Type) {
			case AssetLoader::ShaderAssetDescriptionPropType::TEXTURE:
				return ShaderJSONDataInputPropType::SHADER_PROP_TEXTURE;
			case AssetLoader::ShaderAssetDescriptionPropType::FLOAT:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT;
			case AssetLoader::ShaderAssetDescriptionPropType::VECTOR2:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT2;
			case AssetLoader::ShaderAssetDescriptionPropType::VECTOR3:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT3;
			case AssetLoader::ShaderAssetDescriptionPropType::VECTOR4:
				return ShaderJSONDataInputPropType::SHADER_PROP_FLOAT4;
			case AssetLoader::ShaderAssetDescriptionPropType::BOOL:
				return ShaderJSONDataInputPropType::SHADER_PROP_BOOL;
			case AssetLoader::ShaderAssetDescriptionPropType::INT:
				return ShaderJSONDataInputPropType::SHADER_PROP_INT;
			default:
				throw std::runtime_error("Unknown shader prop type");
			}
		}();
		ShaderJSONDataInputProp prop = {};
		size_t propNameLen = item->Name.length();
		char* propName = new char[propNameLen + 1];
		item->Name.copy(propName, propNameLen);
		propName[propNameLen] = '\0';
		prop.Name = propName;
		prop.Type = type;
		prop.Slot = item->Slot;
		info.Data.ConstantBufferLayout[x] = prop;
	}

	auto nameLen = shader.Name.size();
	auto name = new char[nameLen + 1];
	shader.Name.copy(name, nameLen);
	name[nameLen] = '\0';
	info.Data.Name = name;
	info.Data.ConstantsLen = shader.Description.Constants.size();
	info.Data.ConstantBufferLayoutLen =
		shader.Description.ConstantBufferLayout.size();

	return info;
}

// Loads a shader and compiles it
uint64_t Engine_LoadShaderGPU(ShaderInfo& info) {
	Renderer::GraphicsShader shader = {};
	shader.Code = info.Data.Code;
	shader.Name = std::string(info.Data.Name);
	shader.IsLit =
		info.Data.Lighting == ShaderJSONDataLightingModel::DEFAULT ? true : false;

	switch (info.Data.Format) {
	case SHADER_OUTPUT_RGBA_FLOAT:
		shader.Format = Renderer::TextureFormat::RGBA;
		break;
	case SHADER_OUTPUT_RGBA_UINT:
		shader.Format = Renderer::TextureFormat::RGBA_UINT;
		break;
	default:
		break;
	}

	shader.Constants = {};
	shader.ConstantBufferLayout = {};

	for (int x = 0; x < info.Data.ConstantsLen; x++) {
		Renderer::GraphicsShaderSlotType type = [&]() {
			switch (info.Data.Constants[x].Type) {
			case SHADER_PROP_TEXTURE:
				return Renderer::GraphicsShaderSlotType::TEXTURE;
			case SHADER_PROP_FLOAT:
				return Renderer::GraphicsShaderSlotType::FLOAT;
			case SHADER_PROP_FLOAT2:
				return Renderer::GraphicsShaderSlotType::VECTOR2;
			case SHADER_PROP_FLOAT3:
				return Renderer::GraphicsShaderSlotType::VECTOR3;
			case SHADER_PROP_FLOAT4:
				return Renderer::GraphicsShaderSlotType::VECTOR4;
			case SHADER_PROP_INT:
				return Renderer::GraphicsShaderSlotType::INT;
			case SHADER_PROP_BOOL:
				return Renderer::GraphicsShaderSlotType::BOOL;
			default:
				throw std::runtime_error("Unknown shader prop type");
			}
		}();
		Renderer::GraphicsShaderSlot slot = {};
		slot.Name = info.Data.Constants[x].Name;
		slot.Index = info.Data.Constants[x].Slot;
		slot.Type = type;
		shader.Constants.push_back(slot);
	}

	for (int x = 0; x < info.Data.ConstantBufferLayoutLen; x++) {
		Renderer::GraphicsShaderSlotType type = [&]() {
			switch (info.Data.ConstantBufferLayout[x].Type) {
			case SHADER_PROP_TEXTURE:
				return Renderer::GraphicsShaderSlotType::TEXTURE;
			case SHADER_PROP_FLOAT:
				return Renderer::GraphicsShaderSlotType::FLOAT;
			case SHADER_PROP_FLOAT2:
				return Renderer::GraphicsShaderSlotType::VECTOR2;
			case SHADER_PROP_FLOAT3:
				return Renderer::GraphicsShaderSlotType::VECTOR3;
			case SHADER_PROP_FLOAT4:
				return Renderer::GraphicsShaderSlotType::VECTOR4;
			case SHADER_PROP_INT:
				return Renderer::GraphicsShaderSlotType::INT;
			case SHADER_PROP_BOOL:
				return Renderer::GraphicsShaderSlotType::BOOL;
			default:
				throw std::runtime_error("Unknown shader prop type");
			}
		}();
		Renderer::GraphicsShaderSlot slot = {};
		slot.Name = info.Data.ConstantBufferLayout[x].Name;
		slot.Index = info.Data.ConstantBufferLayout[x].Slot;
		slot.Type = type;
		shader.ConstantBufferLayout.push_back(slot);
	}

	auto index = Instance.Renderer->UploadShaderData(shader);

	return index;
}

// Creates a new material in the renderpipeline and returns its ref
uint64_t Engine_LoadMaterialGPU(const char* name, uint64_t shader) {
	return Instance.Renderer->CreateMaterial(name, shader);
}

#pragma endregion
