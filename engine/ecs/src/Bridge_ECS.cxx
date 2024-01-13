#include "ecs/Bridge_ECS.h"
#include "ecs/EntityRegistry.hxx"
#include "rendering/IMeshRenderer.hxx"

void Bridge_ECS_Init(NativePointer meshRenderer) {
	auto meshRendererPtr = reinterpret_cast<rendering::IMeshRenderer*>(meshRenderer);
	ecs::EntityRegistry::Init(meshRendererPtr);
}

uint64_t Bridge_ECS_CreateEntity() {	
	return ecs::EntityRegistry::CreateEntity();
}

void Bridge_ECS_DestroyEntity(uint64_t entity) {
	ecs::EntityRegistry::DestroyEntity(entity);
}

void Bridge_ECS_AddComponent(uint64_t entity, uint64_t component) {
	ecs::EntityRegistry::AddComponent(entity, component);
}

void Bridge_ECS_RemoveComponent(uint64_t entity, uint64_t component) {

}

uint64_t Bridge_ECS_RegisterComponent(size_t size) {
	return 0;
	// TODO: Implement this function and return the component ID
}