#pragma once

#include <glm/glm.hpp>

namespace kyanite::engine::rendering {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uvs;
	};
}
