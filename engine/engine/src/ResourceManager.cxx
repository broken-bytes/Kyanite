#include "engine/ResourceManager.hxx"
#include "audio/Audio.hxx"
#include "rendering/Rendering.hxx"

namespace kyanite::engine {
	ResourceManager::ResourceManager() {
	}

	ResourceManager::~ResourceManager() {
	}

	auto ResourceManager::LoadTexture(std::string_view path) -> rendering::Texture {
		return rendering::LoadTexture(path);
	}

	auto ResourceManager::LoadAudioClip(std::string_view path) -> audio::AudioClip {
		return audio::LoadAudioClip(path);
	}

	auto ResourceManager::LoadShader(std::string_view path) -> rendering::Shader {
		return rendering::LoadShader(path, path);
	}

	auto ResourceManager::LoadModel(std::string_view path) -> std::vector<rendering::Mesh> {
		return rendering::LoadModel(path);
	}
}