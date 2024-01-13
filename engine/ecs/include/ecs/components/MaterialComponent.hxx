#pragma once

#include <core/Material.hxx>

#include <stdint.h>

namespace ecs::components {
	struct MaterialComponent {
		core::Material& material;
	};
}