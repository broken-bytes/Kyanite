#pragma once 

#include <glm/glm.hpp>

namespace ecs::components
{
	struct TransformComponent {
		glm::vec4 position;
		glm::vec4 rotation;
		glm::vec3 scale;
	};
}