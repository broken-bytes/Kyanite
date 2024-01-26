#include "engine/Bridge_Engine.h"
#include "engine/Engine.hxx"
#include "engine/ecs/EntityRegistry.hxx"
#include "engine/VendorSerializers/GlmSerializers.hxx"
#include <assetpackages/AssetPackages.hxx>
#include <assetpackages/IAssetLoader.hxx>
#include <rendering/IMeshRenderer.hxx>
#include <rendering/Rendering.hxx>
#include <core/Core.hxx>
#include <core/Logger.hxx>
#include <core/ILogger.hxx>

#include <SDL2/SDL.h>

#include <memory>

std::unique_ptr<kyanite::engine::Engine> engine;

void Bridge_Engine_Init(NativePointer window, NativePointer assetLoader, enum Mode mode, NativePointer logger) {
	engine = std::make_unique<kyanite::engine::Engine>();
	if(window != nullptr) {
		engine->window = window;
	} else {
		SDL_CreateWindow("Kyanite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
	}
	auto assetLoaderPtr = reinterpret_cast<kyanite::engine::assetpackages::IAssetLoader*>(assetLoader);
	auto loggerPtr = reinterpret_cast<kyanite::engine::core::ILogger*>(logger);

	engine->assetLoader = std::shared_ptr<kyanite::engine::assetpackages::IAssetLoader>(assetLoaderPtr);
	engine->logger = std::shared_ptr<kyanite::engine::core::ILogger>(loggerPtr);
	engine->renderer = std::make_unique<rendering::Renderer>();

	// Initilize subsystems
	kyanite::engine::core::InitCore();
	kyanite::engine::core::logger::SetLogger(engine->logger);
	kyanite::engine::assetpackages::Initialize(engine->assetLoader);
	kyanite::engine::rendering::Init(window);

	auto meshRendererPtr = reinterpret_cast<kyanite::engine::rendering::IMeshRenderer*>(engine->renderer.get());
	ecs::EntityRegistry::Init(meshRendererPtr);
}

uint64_t Bridge_Engine_CreateEntity() {	
	return ecs::EntityRegistry::CreateEntity();
}

void Bridge_Engine_DestroyEntity(uint64_t entity) {
	ecs::EntityRegistry::DestroyEntity(entity);
}

void Bridge_Engine_AddComponent(uint64_t entity, uint64_t component) {
	ecs::EntityRegistry::AddComponent(entity, component);
}

void Bridge_Engine_RemoveComponent(uint64_t entity, uint64_t component) {

}

uint64_t Bridge_Engine_RegisterComponent(size_t size) {
	return 0;
	// TODO: Implement this function and return the component ID
}

void Bridge_Engine_RegisterSystem(void* systemFuncPtr) {

}