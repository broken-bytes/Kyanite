#pragma once

#include <core/Mesh.hxx>

#include <stdint.h>

namespace ecs::components {
	struct MeshComponent {
		core::Mesh& mesh;
	};
}