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

#ifdef __cplusplus
extern "C" {
#endif

struct Vector3Component {
  float x;
  float y;
  float z;
} typedef Vector3Component;

struct TransformComponent {
  Vector3Component Position;
  Vector3Component Rotation;
  Vector3Component Scale;
} typedef TransformComponent;

struct MeshComponent {
  uint64_t internalId;
} typedef MeshComponent;

struct MaterialComponent {
  uint64_t internalId;
} typedef MaterialComponent;

void Flecs_Init(uint8_t);
void Flecs_Update(float frametime);
ecs_world_t* Flecs_GetWorld(void);
ecs_entity_t Flecs_GetScene(void);
uint64_t Flecs_CreateEntity(const char* name);
void Flecs_DeleteEntity(uint64_t id);
uint64_t Flecs_RegisterComponent(uint64_t size, uint8_t alignment, const char *uuid);
uint64_t Flecs_AddComponent(uint64_t entity, uint64_t id, uint64_t size, void *data);
const void* Flecs_GetComponent(uint64_t entity, uint64_t id);
uint64_t Flecs_RegisterSystem(const char* name, void *system, bool multiThreaded, uint64_t *componentIds, size_t numComponents);
void* Flecs_GetComponentData(void* iterator, size_t size, uint8_t index, size_t* count);
void Flecs_GetSystemDeltaTime(void* iterator, float* deltaTime);
uint64_t Flecs_GetSystemFromIter(void* iterator);
#ifdef __cplusplus
}
#endif