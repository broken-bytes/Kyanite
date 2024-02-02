#include "ecs/EntityRegistry.hxx"
#include <core/Core.hxx>

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
	
	auto Init(bool debugServer) -> void {

		// If we are in debug mode, we want to start the debug server and load all component metadata
		if (debugServer) {
			world.set<flecs::Rest>({ });
			world.import<flecs::monitor>();
			auto metafiles = kyanite::engine::core::GetFileList(std::filesystem::current_path() / "meta");
			for (auto& file : metafiles) {
				auto data = kyanite::engine::core::LoadFileToBuffer(file.path().string());
				std::string value(data.begin(), data.end());
				world.plecs_from_str((file.path().stem()).string().c_str(), value.c_str());
			}
		}
	}

	auto GetRegistry() -> ecs_world_t* {
		return world;
	}

	auto CreateEntity(std::string name) -> ecs_entity_t {
		return world.entity(name.c_str());
	}

	auto DestroyEntity(ecs_entity_t entity) -> void {
		world.entity(entity).destruct();
	}

	auto CreateComponent(std::string name, size_t size, size_t alignment) -> ecs_entity_t {
		auto desc = ecs_component_desc_t {};
		desc.type = {};
		desc.type.size = size;
		desc.type.alignment = alignment;
		desc.type.name = name.c_str();

		ecs_entity_desc_t entityDesc = {};
		entityDesc.name = name.c_str();
		desc.entity = ecs_entity_init(world, &entityDesc);
		
		return ecs_component_init(world, &desc);
	}

	auto AddComponent(ecs_entity_t entity, ecs_entity_t component) -> void {
		ecs_add_id(world, entity, component);
	}

	auto SetComponent(ecs_entity_t entity, ecs_entity_t component, void* data) -> void {
		world.entity(entity).set_ptr(component, data);
	}

	auto RemoveComponent(ecs_entity_t entity, ecs_entity_t component) -> void {
		ecs_remove_id(world, entity, component);
	}

	auto Update(float delta) -> void {
		world.progress();
	}

	auto RegisterSystem(std::string name, std::vector<ecs_entity_t> filter, void (*func)(ecs_iter_t* it)) -> ecs_entity_t {
		ecs_system_desc_t desc = {};
		ecs_entity_desc_t entityDesc = {};
		entityDesc.name = name.c_str();
		entityDesc.add[0] = ecs_pair(EcsDependsOn, EcsOnUpdate);
		ecs_entity_t system = ecs_entity_init(world, &entityDesc);
		desc.entity = system;
		desc.callback = func;
		for (int x = 0; x < filter.size(); x++) {
			desc.query.filter.terms[x].id = filter[x];
			desc.query.filter.terms[x].oper = EcsAnd;
		}

		
		auto sysId = ecs_system_init(world, &desc);

		return sysId;
	}

	auto GetComponentBuffer(ecs_iter_t* iter, uint8_t index, size_t componentSize) -> void* {
		return ecs_field_w_size(iter, componentSize, index);
	}
}