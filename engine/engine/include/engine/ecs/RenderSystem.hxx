#pragma once

namespace ecs::systems {
	class RenderSystem {
	public:
		RenderSystem();
		~RenderSystem();
		auto Tick(float delta) -> void;
	};
}