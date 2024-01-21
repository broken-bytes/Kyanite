#pragma once

#include <glm/glm.hpp>

namespace kyanite::engine::rendering {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UVs;
	};
}
