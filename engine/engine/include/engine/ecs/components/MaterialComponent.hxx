#pragma once

#include <rendering/Material.hxx>

#include <stdint.h>

namespace ecs::components {
	struct MaterialComponent {
		kyanite::engine::rendering::Material& material;
	};
}