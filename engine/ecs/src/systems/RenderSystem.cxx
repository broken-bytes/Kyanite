#include "ecs/systems/RenderSystem.hxx"
#include "ecs/EntityRegistry.hxx"
#include "ecs/components/MeshComponent.hxx"
#include "ecs/components/RenderableComponent.hxx"
#include "ecs/components/TransformComponent.hxx"

#include <flecs.h>

#include <functional>

namespace ecs::systems {
	RenderSystem::RenderSystem(rendering::IMeshRenderer* renderer) : _renderer(renderer) {
		auto transformDesc = ecs_component_desc_t {};
		transformDesc.type = {};
		transformDesc.type.size = sizeof(ecs::components::TransformComponent);
		transformDesc.type.alignment = alignof(ecs::components::TransformComponent);
		auto transformComponent = ecs_component_init(ecs::EntityRegistry::GetRegistry(), &transformDesc);

		auto meshDesc = ecs_component_desc_t{};
		meshDesc.type = {};
		meshDesc.type.size = sizeof(ecs::components::MeshComponent);
		meshDesc.type.alignment = alignof(ecs::components::MeshComponent);
		auto meshComponent = ecs_component_init(ecs::EntityRegistry::GetRegistry(), &meshDesc);

		auto renderableDesc = ecs_component_desc_t{};
		renderableDesc.type = {};
		renderableDesc.type.size = sizeof(ecs::components::RenderableComponent);
		renderableDesc.type.alignment = alignof(ecs::components::RenderableComponent);
		auto renderableComponent = ecs_component_init(ecs::EntityRegistry::GetRegistry(), &renderableDesc);

        
		
		auto renderAction = std::mem_fn(&RenderSystem::Render);
		auto desc = ecs_system_desc_t {};
		desc.multi_threaded = true;
		desc.entity = {};
		desc.callback = ecs_iter_action_t(&renderAction);
		desc.tick_source = EcsOnUpdate;
		desc.query.filter = {};
		desc.query.filter.terms[0] = { transformComponent };
		desc.query.filter.terms[1] = { meshComponent };
		desc.query.filter.terms[2] = { renderableComponent };

		_system = ecs_system_init(ecs::EntityRegistry::GetRegistry(), &desc);
	}

	RenderSystem::~RenderSystem() {
		ecs_delete(ecs::EntityRegistry::GetRegistry(), _system);
	}

	auto RenderSystem::Tick(float dt) -> void {
		
	}

	auto RenderSystem::Render(ecs_iter_t* iter) -> void {
		auto transformComponents = ecs_field(iter, ecs::components::TransformComponent, 1);
		auto meshComponents = ecs_field(iter, ecs::components::MeshComponent, 2);
		auto renderableComponents = ecs_field(iter, ecs::components::RenderableComponent, 3);

		for (auto i = 0; i < iter->count; ++i) {
			auto transformComponent = transformComponents[i];
			auto meshComponent = meshComponents[i];
			auto renderableComponent = renderableComponents[i];

			_renderer->RenderMesh(
				meshComponent.mesh,
				transformComponent.position,
				transformComponent.rotation,
				transformComponent.scale
			);
		}
	}
}