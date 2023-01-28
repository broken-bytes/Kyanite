#pragma once

#define FLECS_CUSTOM_BUILD // Don't build all addons
#define FLECS_SYSTEM
#define FLECS_META
#define FLECS_META_C
#define FLECS_EXPR
#define FLECS_SNAPSHOT
#define FLECS_LOG
#define FLECS_PIPELINE
#define FLECS_MONITOR
#define FLECS_STATS
#define FLECS_REST
#include <flecs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	struct Vector3Component {
		float x;
		float y;
		float z;
	} typedef Vector3Component;

	struct Vector4Component {
		float w;
		float x;
		float y;
		float z;
	} typedef Vector4Component;

	struct TransformComponent {
		Vector3Component Position;
		Vector4Component Rotation;
		Vector3Component Scale;
	} typedef TransformComponent;

	struct MeshComponent {
		uint64_t internalId;
	} typedef MeshComponent;

	struct MaterialComponent {
		uint64_t internalId;
	} typedef MaterialComponent;

	struct RigidBodyComponent {
		uint8_t IsStatic;
		float Mass;
	} typedef RigidBodyComponent;

	struct BoxColliderComponent {
		Vector3Component Bounds;
	} typedef BoxColliderComponent;

	struct SphereColliderComponent {
		float Radius;
	} typedef SphereColliderComponent;

	struct CapsuleColliderComponent {
		float Radius;
		float HalfHeight;
	} typedef CapsuleColliderComponent;

	void Flecs_Init(uint8_t);
	void Flecs_Update(float frametime);
	ecs_world_t* Flecs_GetWorld(void);
	ecs_entity_t Flecs_GetScene(void);
	uint64_t Flecs_CreateEntity(const char* name);
	void Flecs_DeleteEntity(uint64_t id);
	uint64_t Flecs_RegisterComponent(uint64_t size, uint8_t alignment, const char* uuid);
	uint64_t Flecs_AddComponent(uint64_t entity, uint64_t id, uint64_t size, void* data);
	void* Flecs_GetComponent(uint64_t entity, uint64_t id);
	uint64_t Flecs_RegisterSystem(const char* name, void* system, bool multiThreaded, uint64_t* componentIds, size_t numComponents);
	void* Flecs_GetComponentData(void* iterator, size_t size, uint8_t index, size_t* count);
	void Flecs_GetSystemDeltaTime(void* iterator, float* deltaTime);
	uint64_t Flecs_GetSystemFromIter(void* iterator);
	bool Flecs_CheckIfCollider(uint64_t componentId);
	bool Flecs_CheckIfRigidBody(uint64_t componentId);
#ifdef __cplusplus
}
#endif