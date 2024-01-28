#include "engine/ecs/EntityRegistry.hxx"
#include "engine/ecs/systems/RenderSystem.hxx"
#include <rendering/IMeshRenderer.hxx>

#define FLECS_CORE
#define FLECS_SYSTEM
#define FLECS_PIPELINE
#define FLECS_META
#define FLECS_META_C
#define FLECS_PIPELINE
#define FLECS_HTTP
#define FLECS_TIMER
#define FLECS_REST
#define FLECS_STATS
#define FLECS_MONITOR
#define FLECS_CPP
#include <flecs.h>
#include <flecs/addons/rest.h>

#include <memory>

namespace ecs::EntityRegistry {
	flecs::world world;
	std::unique_ptr<systems::RenderSystem> renderSystem;
	
	auto Init(rendering::IMeshRenderer* renderer, bool debugServer) -> void {
		renderSystem = std::make_unique<systems::RenderSystem>(renderer);
		world.set<flecs::Rest>({ });
		world.import<flecs::monitor>();
	}

	auto GetRegistry() -> ecs_world_t* {
		return world;
	}

	auto CreateEntity(std::string name) -> ecs_entity_t {
		return world.entity(name.c_str());
	}

	auto DestroyEntity(ecs_entity_t entity) -> void {
		world.delete_with(entity);
	}

	auto CreateComponent(std::string name, size_t size, size_t alignment) -> ecs_entity_t {
		auto desc = ecs_component_desc_t {};
		desc.type = {};
		desc.type.size = size;
		desc.type.alignment = alignment;
		desc.type.name = name.c_str();
		
		return ecs_component_init(world, &desc);
	}

	auto AddComponent(ecs_entity_t entity, ecs_entity_t component) -> void {
		ecs_add_id(world, entity, component);
	}

	auto RemoveComponent(ecs_entity_t entity, ecs_entity_t component) -> void {
		ecs_remove_id(world, entity, component);
	}

	auto Update(float delta) -> void {
		world.progress();
	}
}