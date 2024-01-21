#include "engine/Bridge_Engine.h"
#include "engine/ecs/EntityRegistry.hxx"
#include "rendering/IMeshRenderer.hxx"
#include "engine/VendorSerializers/GlmSerializers.hxx"

void Bridge_Engine_Init(NativePointer meshRenderer) {
	auto meshRendererPtr = reinterpret_cast<kyanite::engine::rendering::IMeshRenderer*>(meshRenderer);
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