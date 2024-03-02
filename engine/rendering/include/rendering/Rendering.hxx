#pragma once

#include "Mesh.hxx"
#include "Renderer.hxx"
#include "Shader.hxx"
#include "Texture.hxx"
#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <imgui.h>

#include <string>
#include <vector>

namespace kyanite::engine::rendering {
	// Lifecycle
	auto Init(NativePointer window, ImGuiContext* imGuiContext) -> void;
	auto Shutdown() -> void;
    auto PreFrame() -> void;
	auto Update(float deltaTime) -> void;
	auto PostFrame() -> void;
	
	// Resource creation
	auto LoadTexture(std::string_view path) -> uint32_t;
	auto LoadShader(std::string code, ShaderType type) -> uint32_t;
	auto LoadModel(std::string_view path) -> std::vector<Mesh>;
	auto CreateVertexBuffer(const void* data, size_t size) -> uint32_t;
	auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> uint32_t;
	auto UpdateVertexBuffer(uint32_t buffer, const void* data, size_t size) -> void;
	auto UpdateIndexBuffer(uint32_t buffer, const void* data, size_t size) -> void;

	// Resource destruction
	auto UnloadShader(uint64_t shader) -> void;

	// State management
	auto SetClearColor(float r, float g, float b, float a) -> void;
	auto SetViewport(int x, int y, int width, int height) -> void;
	auto DrawIndexed(uint32_t vertexBuffer, uint32_t indexBuffer, uint32_t material) -> void;
}