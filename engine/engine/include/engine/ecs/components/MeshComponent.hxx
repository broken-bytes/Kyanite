#pragma once

#include <rendering/Mesh.hxx>

#include <stdint.h>

namespace ecs::components {
	struct MeshComponent {
		kyanite::engine::rendering::Mesh& mesh;
	};
}