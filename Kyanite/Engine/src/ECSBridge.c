#include "ECSBridge.h"

ecs_world_t *ECS;
ecs_entity_t Scene;

void ECS_Init(uint8_t numThreads) {
  ECS = ecs_init();
  ECS_IMPORT(ECS, FlecsMonitor);
  ecs_set_threads(ECS, numThreads);
  Scene = ecs_new_id(ECS);
  ecs_singleton_set(ECS, EcsRest, {0});
  ecs_entity_t sceneTag = ecs_new_id(ECS);

  ecs_add_id(ECS, Scene, sceneTag);
  ecs_set_name(ECS, Scene, "Scene");
}

void ECS_Update(float frametime) {
    ecs_progress(ECS, frametime);
}

ecs_world_t* ECS_GetWorld(void) {
    return ECS;
}

ecs_entity_t ECS_GetScene(void) {
    return Scene;
}

uint64_t ECS_CreateEntity(const char *name) {
    ecs_entity_t e = ecs_new_w_pair(ECS, EcsChildOf, Scene);
    ecs_set_name(ECS, e, name);
    return e;
}

uint64_t ECS_RegisterComponent(uint64_t size, uint8_t alignment, const char *uuid) {
  return ecs_component_init(ECS, &(ecs_component_desc_t){
        .type.size = size,
        .type.alignment = alignment,
        .type.name = uuid,
        .entity = ecs_entity(ECS, { .name = uuid })
    });
}

uint64_t ECS_AddComponent(uint64_t entity, uint64_t id, uint64_t size, void *data) {
     ecs_set_id(ECS, entity, id, size, data);
  return 0;
}

const void* ECS_GetComponent(uint64_t entity, uint64_t id) {
    return ecs_get_id(ECS, entity, id);
}

uint64_t ECS_RegisterSystem(const char* name, void *system, uint64_t *componentIds, size_t numComponents) {
  ecs_system_desc_t desc = {0};
  desc.entity = ecs_entity(ECS, { 
    .name = name,
    .add = { ecs_dependson(EcsOnUpdate)}
  });
  typedef void (*iterFuncPtr)(ecs_iter_t *);
  desc.callback = (iterFuncPtr)(system);
  for(int x = 0; x < numComponents; x++) {
    desc.query.filter.terms[x].id = componentIds[x];
    desc.query.filter.terms[x].name = ecs_get_name(ECS, componentIds[x]);
  }

  return ecs_system_init(ECS, &desc);
}

void* ECS_GetComponentData(void* iterator, size_t size, uint8_t index) {
    return ecs_field_w_size((ecs_iter_t*)iterator, size, index);
}