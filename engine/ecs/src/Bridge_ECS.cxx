#include "ecs/Bridge_ECS.h"
#include "ecs/EntityRegistry.hxx"

#include <map>
#include <memory>
#include <Windows.h>

// Store all the component types in a map of int -> uint64_t
// The int is the hash value coming from swift and the uint64_t is the component ID
// This is so we can easily get the component ID from the hash value
std::map<int64_t, uint64_t> componentTypes;

void ECS_Init(NativePointer logger, bool isDebug) {
	ecs::EntityRegistry::Init(isDebug);
}

void ECS_Update(float delta) {
	ecs::EntityRegistry::Update(delta);
}

uint64_t ECS_CreateEntity(const char* name) {
	return ecs::EntityRegistry::CreateEntity(name);
}

void ECS_DestroyEntity(uint64_t entity) {
	ecs::EntityRegistry::DestroyEntity(entity);
}

void ECS_AddComponent(uint64_t entity, uint64_t component) {
	ecs::EntityRegistry::AddComponent(entity, component);
}

void ECS_SetComponent(uint64_t entity, uint64_t component, void* data) {
	ecs::EntityRegistry::SetComponent(entity, component, data);
}

void ECS_RemoveComponent(uint64_t entity, uint64_t component) {

}

uint64_t ECS_RegisterComponent(const char* name, size_t size, size_t alignment) {
	return ecs::EntityRegistry::CreateComponent(name, size, alignment);
}

uint64_t ECS_RegisterSystem(const char* name, uint64_t* filter, size_t filterLen, void (*func)(NativePointer)) {
	std::vector<uint64_t> filterVec(filter, filter + filterLen);
	return ecs::EntityRegistry::RegisterSystem(name, filterVec, reinterpret_cast<void (*)(ecs_iter_t*)>(func));
}

NativePointer ECS_GetComponentsFromIterator(NativePointer iterator, uint8_t index, size_t componentSize) {
	auto iter = reinterpret_cast<ecs_iter_t*>(iterator);

	return ecs::EntityRegistry::GetComponentBuffer(iter, index, componentSize);
}

size_t ECS_GetIteratorSize(NativePointer iterator) {
	return reinterpret_cast<ecs_iter_t*>(iterator)->count;
}