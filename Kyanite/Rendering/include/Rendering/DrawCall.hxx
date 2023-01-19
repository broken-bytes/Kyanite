#pragma once

#include <cstdint>

#include "DescriptorHandle.hxx"
#include "Renderer.hxx"
#include "glm/glm.hpp"

namespace Renderer {
    struct DrawCall {
		std::uint64_t EntityId;
		std::uint64_t MeshId;
		std::uint64_t MaterialId;
		glm::mat4 Transform;
		RendererLayer RenderLayer;
		GPUDescriptorHandle* RenderTarget;
	};
}