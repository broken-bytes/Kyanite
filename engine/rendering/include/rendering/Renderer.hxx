#pragma once

#include "Material.hxx"
#include "Mesh.hxx"
#include "Texture.hxx"
#include "IMeshRenderer.hxx"
#include <shared/Exported.hxx>

#include <glm/glm.hpp>

namespace kyanite::engine::rendering {
	class Renderer: public IMeshRenderer {
	public:
		Renderer();
		~Renderer();
		auto Render();
		auto Update();
		auto UploadMesh(const Mesh& mesh);
		auto UploadMaterial(const Material& material);
		auto UploadTexture(const Texture& texture);
		auto RenderMesh(Mesh& mesh, Material& material, glm::vec4 pos, glm::vec4 rotation, glm::vec3 scale) -> void override;
	};
}