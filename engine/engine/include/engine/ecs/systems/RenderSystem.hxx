#pragma once

#include <rendering/IMeshRenderer.hxx>
#include <flecs.h>

namespace ecs::systems {
	class RenderSystem {
	public:
		RenderSystem(kyanite::engine::rendering::IMeshRenderer* renderer);
		~RenderSystem();
		auto Tick(float dt) -> void;

	private:
		ecs_entity_t _system;
		kyanite::engine::rendering::IMeshRenderer* _renderer;

		auto Render(ecs_iter_t* iter) -> void;
	};
}