#include "engine/ecs/EntityRegistry.hxx"
#include "engine/ecs/systems/RenderSystem.hxx"
#include <rendering/IMeshRenderer.hxx>

#include <flecs.h>

#include <memory>

namespace ecs::EntityRegistry {
	ecs_world_t* world;
	std::unique_ptr<systems::RenderSystem> renderSystem;
	
	auto Init(rendering::IMeshRenderer* renderer) -> void {
		world = ecs_init();
		renderSystem = std::make_unique<systems::RenderSystem>(renderer);
	}

	auto GetRegistry() -> ecs_world_t* {
		return world;
	}

	auto CreateEntity() -> ecs_entity_t {
		return ecs_new_id(world);
	}

	auto DestroyEntity(ecs_entity_t entity) -> void {
		ecs_delete(world, entity);
	}

	auto CreateComponent(size_t size, size_t alignment) -> ecs_entity_t {
		auto desc = ecs_component_desc_t {};
		desc.type = {};
		desc.type.size = size;
		desc.type.alignment = alignment;
		
		return ecs_component_init(world, &desc);
	}

	auto AddComponent(ecs_entity_t entity, ecs_entity_t component) -> void {
		ecs_add_id(world, entity, component);
	}

	auto RemoveComponent(ecs_entity_t entity, ecs_entity_t component) -> void {
		ecs_remove_id(world, entity, component);
	}
}