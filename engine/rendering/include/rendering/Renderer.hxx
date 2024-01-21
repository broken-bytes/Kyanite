#pragma once

#include "Material.hxx"
#include "Mesh.hxx"
#include "Texture.hxx"

namespace kyanite::engine::rendering {
	class Renderer {
	public:
		Renderer();
		~Renderer();
		auto Render();
		auto Update();
		auto UploadMesh(const Mesh& mesh);
		auto UploadMaterial(const Material& material);
		auto UploadTexture(const Texture& texture);

		static auto Instance() -> Renderer&;
	};
}