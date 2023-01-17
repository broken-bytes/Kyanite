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

void ECS_Init(uint8_t);
void ECS_Update(float frametime);
ecs_world_t* ECS_GetWorld(void);
ecs_entity_t ECS_GetScene(void);

uint64_t ECS_CreateEntity(const char* name);
uint64_t ECS_RegisterComponent(uint64_t size, uint8_t alignment, const char *uuid);
uint64_t ECS_AddComponent(uint64_t entity, uint64_t id, uint64_t size, void *data);
const void* ECS_GetComponent(uint64_t entity, uint64_t id);
uint64_t ECS_RegisterSystem(const char* name, void *system, uint64_t *componentIds, size_t numComponents);
void* ECS_GetComponentData(void* iterator, size_t size, uint8_t index, size_t* count);
#ifdef __cplusplus
}
#endif