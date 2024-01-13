#pragma once

#include <core/Mesh.hxx>
#include <core/Material.hxx>

#include <glm/glm.hpp>

namespace rendering {
	class IMeshRenderer {
	public:
		IMeshRenderer() = default;
		virtual ~IMeshRenderer() = 0;
		virtual void RenderMesh(core::Mesh& mesh, core::Material& material, glm::vec4 pos, glm::vec4 rotation, glm::vec3 scale) = 0;
	};
}