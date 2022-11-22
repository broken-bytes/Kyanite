#pragma once

#include <cstdint>

#include "glm/glm.hpp"

namespace Renderer {
    struct DrawCall {
		std::uint64_t MeshId;
		std::uint64_t MaterialId;
		glm::mat4 Transform;
	};
}