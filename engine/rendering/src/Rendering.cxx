#include "rendering/Rendering.hxx"
#include "rendering/Device.hxx"
#include "core/Core.hxx"

#include <glad/glad.h>
#include <FreeImagePlus.h>

#include <stdexcept>
#include <memory>

namespace core = kyanite::engine::core;

namespace kyanite::engine::rendering {
	std::shared_ptr<Device> device = nullptr;

	auto Init(NativePointer window) -> void {
		FreeImage_Initialise();
	}

	auto Update() -> void {

	}

	auto Render() -> void {

	}

	auto LoadTexture(std::string_view path) -> Texture {
		auto buffer = core::LoadFileToBuffer(path);

		FIMEMORY* memory = FreeImage_OpenMemory(buffer.data(), buffer.size());
		FIBITMAP* image = FreeImage_LoadFromMemory(FIF_UNKNOWN, memory);
		FreeImage_CloseMemory(memory);

		if (image) {
		}
		else {
			throw std::runtime_error("Failed to load image");
		}

		return Texture();
	}

	auto LoadShader(std::string_view vertexPath, std::string_view fragmentPath) -> Shader {
		return Shader();
	}

	auto LoadModel(std::string_view path) -> std::vector<Mesh> {
		return std::vector<Mesh>();
	}
}