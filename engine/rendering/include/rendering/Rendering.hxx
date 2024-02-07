#pragma once

#include "Mesh.hxx"
#include "Renderer.hxx"
#include "Shader.hxx"
#include "Texture.hxx"
#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <string>
#include <vector>

namespace kyanite::engine::rendering {
	auto Init(NativePointer window) -> void;
	auto Shutdown() -> void;
    auto PreFrame() -> void;
	auto Update(float deltaTime) -> void;
	auto PostFrame() -> void;
	auto SetClearColor(float r, float g, float b, float a) -> void;
	auto LoadTexture(std::string_view path) -> Texture;
	auto LoadShader(std::string_view vertexPath, std::string_view fragmentPath) -> Shader;
	auto LoadModel(std::string_view path) -> std::vector<Mesh>;
}