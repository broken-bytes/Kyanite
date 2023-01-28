#include "ECSBridge.h"

ecs_world_t* ECS;
ecs_entity_t Scene;

void SetupBuiltinTypes() {
	ECS_COMPONENT(ECS, Vector3Component);
	ECS_COMPONENT(ECS, Vector4Component);
	ECS_COMPONENT(ECS, TransformComponent);
	ECS_COMPONENT(ECS, MeshComponent);
	ECS_COMPONENT(ECS, MaterialComponent);
	ECS_COMPONENT(ECS, RigidBodyComponent);
	ECS_COMPONENT(ECS, BoxColliderComponent);
	ECS_COMPONENT(ECS, SphereColliderComponent);
	ECS_COMPONENT(ECS, CapsuleColliderComponent);

	ecs_struct(ECS, { .entity = ecs_id(Vector3Component),
					 .members = {{.name = "x", .type = ecs_id(ecs_f32_t)},
								 {.name = "y", .type = ecs_id(ecs_f32_t)},
								 {.name = "z", .type = ecs_id(ecs_f32_t)}} });

	ecs_struct(
		ECS,
		{
			.entity = ecs_id(Vector4Component),
			.members = {
				{.name = "w", .type = ecs_id(ecs_f32_t)},
				{.name = "x", .type = ecs_id(ecs_f32_t)},
				{.name = "y", .type = ecs_id(ecs_f32_t)},
				{.name = "z", .type = ecs_id(ecs_f32_t)}
			}
		}
	);

	ecs_struct(ECS, { .entity = ecs_id(TransformComponent),
					 .members = {
						 {.name = "position", .type = ecs_id(Vector3Component)},
						 {.name = "rotation", .type = ecs_id(Vector4Component)},
						 {.name = "scale", .type = ecs_id(Vector3Component)},
					 } });
	ecs_struct(ECS, { .entity = ecs_id(MeshComponent),
					 .members = {
						 {.name = "internalRefId", .type = ecs_id(ecs_u64_t)},
					 } });
	ecs_struct(ECS, { .entity = ecs_id(MaterialComponent),
				   .members = {
					   {.name = "internalRefId", .type = ecs_id(ecs_u64_t)},
				   } });

	ecs_struct(ECS, { .entity = ecs_id(RigidBodyComponent),
			   .members = {
				   {.name = "IsStatic", .type = ecs_id(ecs_bool_t)},
					{.name = "Mass", .type = ecs_id(ecs_f32_t)}
			   } });
	ecs_struct(ECS, { .entity = ecs_id(BoxColliderComponent),
		   .members = {
			   {.name = "Bounds", .type = ecs_id(Vector3Component)},
			   {.name = "IsStatic", .type = ecs_id(ecs_bool_t)},
		   } });
	ecs_struct(ECS, { .entity = ecs_id(SphereColliderComponent),
	   .members = {
		   {.name = "Radius", .type = ecs_id(ecs_f32_t)},
	   } });
	ecs_struct(ECS, { .entity = ecs_id(CapsuleColliderComponent),
	   .members = {
		   {.name = "Radius", .type = ecs_id(ecs_f32_t)},
		   {.name = "HalfHeight", .type = ecs_id(ecs_f32_t)},
	   } });

}

void Flecs_Init(uint8_t numThreads) {
	ECS = ecs_init();
	ECS_IMPORT(ECS, FlecsMonitor);
	ecs_set_threads(ECS, numThreads);
	Scene = ecs_new_id(ECS);
	ecs_singleton_set(ECS, EcsRest, { 0 });
	ecs_entity_t sceneTag = ecs_new_id(ECS);

	ecs_add_id(ECS, Scene, sceneTag);
	ecs_set_name(ECS, Scene, "Scene");
	SetupBuiltinTypes();
	ecs_log_set_level(2);
}

void Flecs_Update(float frametime) {
	ecs_progress(ECS, frametime);
}

ecs_world_t* Flecs_GetWorld(void) {
	return ECS;
}

ecs_entity_t Flecs_GetScene(void) {
	return Scene;
}

uint64_t Flecs_CreateEntity(const char* name) {
	ecs_entity_t e = ecs_new_w_pair(ECS, EcsChildOf, Scene);
	ecs_set_name(ECS, e, name);
	return e;
}

void Flecs_DeleteEntity(uint64_t id) {
	ecs_delete(ECS, id);
}

uint64_t Flecs_RegisterComponent(uint64_t size, uint8_t alignment, const char* uuid) {
	return ecs_component_init(ECS, &(ecs_component_desc_t){
		.type.size = size,
			.type.alignment = alignment,
			.type.name = uuid,
			.entity = ecs_entity(ECS, { .name = uuid })
	});
}

uint64_t Flecs_AddComponent(uint64_t entity, uint64_t id, uint64_t size, void* data) {
	ecs_set_id(ECS, entity, id, size, data);
	return 0;
}

void* Flecs_GetComponent(uint64_t entity, uint64_t id) {
	return ecs_get_mut_id(ECS, entity, id);
}

uint64_t Flecs_RegisterSystem(const char* name, void* system, bool multiThreaded, uint64_t* componentIds, size_t numComponents) {
	ecs_system_desc_t desc = { 0 };
	desc.multi_threaded = multiThreaded;
	desc.entity = ecs_entity(ECS, {
	  .name = name,
	  .add = { ecs_dependson(EcsOnUpdate)}
		});
	typedef void (*iterFuncPtr)(ecs_iter_t*);
	desc.callback = (iterFuncPtr)(system);
	for (int x = 0; x < numComponents; x++) {
		desc.query.filter.terms[x].id = componentIds[x];
		desc.query.filter.terms[x].name = ecs_get_name(ECS, componentIds[x]);
	}

	return ecs_system_init(ECS, &desc);
}

void* Flecs_GetComponentData(void* iterator, size_t size, uint8_t index, size_t* count) {
	ecs_iter_t* it = (ecs_iter_t*)iterator;
	*count = it->count;
	return ecs_field_w_size((ecs_iter_t*)iterator, size, index);
}

void Flecs_GetSystemDeltaTime(void* iterator, float* deltaTime) {
	*deltaTime = ((ecs_iter_t*)iterator)->delta_time;
}

uint64_t Flecs_GetSystemFromIter(void* iterator) {
	return ((ecs_iter_t*)iterator)->system;
}

bool Flecs_CheckIfCollider(uint64_t componentId) {
	ECS_COMPONENT(ECS, RigidBodyComponent);
	ECS_COMPONENT(ECS, BoxColliderComponent);
	ECS_COMPONENT(ECS, SphereColliderComponent);
	ECS_COMPONENT(ECS, CapsuleColliderComponent);

	if (
		componentId == ecs_id(BoxColliderComponent) ||
		componentId == ecs_id(SphereColliderComponent) ||
		componentId == ecs_id(CapsuleColliderComponent)
		) {
		return true;
	}

	return false;
}

bool Flecs_CheckIfRigidBody(uint64_t componentId) {
	ECS_COMPONENT(ECS, RigidBodyComponent);

	return componentId == ecs_id(RigidBodyComponent);
}
