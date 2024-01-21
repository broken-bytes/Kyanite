#pragma once

#include "audio/AudioClip.hxx"
#include "rendering/Mesh.hxx"
#include "rendering/Model.hxx"
#include "rendering/Shader.hxx"
#include "rendering/Texture.hxx"

#include <string>
#include <vector>

namespace kyanite::engine {
	class ResourceManager {
	public:
		ResourceManager();
		~ResourceManager();

		template<typename T>
		auto LoadResource(std::string_view path) -> T {
			switch (T) {
			case rendering::Texture:
				return LoadTexture(path);
			case audio::AudioClip:
				return LoadAudioClip(path);
			case rendering::Shader:
				return LoadShader(path);
			case rendering::Model:
				return LoadModel(path);
			default:
				return nullptr;
			}
		}

	private:
		auto LoadTexture(std::string_view path) -> rendering::Texture;
		auto LoadAudioClip(std::string_view path) -> audio::AudioClip;
		auto LoadShader(std::string_view path) -> rendering::Shader;
		auto LoadModel(std::string_view path) -> std::vector<rendering::Mesh>;
	};
}