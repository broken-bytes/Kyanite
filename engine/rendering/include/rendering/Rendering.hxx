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
	EXPORTED auto Init(NativePointer window) -> void;
	EXPORTED auto Update() -> void;
	EXPORTED auto Render() -> void;
	EXPORTED auto SetClearColor(float r, float g, float b, float a) -> void;
	EXPORTED auto LoadTexture(std::string_view path) -> Texture;
	EXPORTED auto LoadShader(std::string_view vertexPath, std::string_view fragmentPath) -> Shader;
	EXPORTED auto LoadModel(std::string_view path) -> std::vector<Mesh>;
}