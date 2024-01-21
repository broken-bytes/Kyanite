#pragma once

#include "Mesh.hxx"
#include "Material.hxx"

#include <glm/glm.hpp>

namespace kyanite::engine::rendering {
	class IMeshRenderer {
	public:
		IMeshRenderer() = default;
		virtual ~IMeshRenderer() = 0;
		virtual void RenderMesh(Mesh& mesh, Material& material, glm::vec4 pos, glm::vec4 rotation, glm::vec3 scale) = 0;
	};
}