#pragma once

#include <cstdint>

#include "glm/glm.hpp"

namespace Renderer {
    struct DrawCall {
		std::uint64_t MeshId;
		std::uint64_t ShaderId;
		std::uint64_t TextureId;
		glm::mat4 Transform;
		
	};
}