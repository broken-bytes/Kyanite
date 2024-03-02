#pragma once

#include "CommandList.hxx"
#include "CommandListType.hxx"
#include "CommandQueue.hxx"
#include "Fence.hxx"
#include "Shader.hxx"
#include "Swapchain.hxx"
#include "IndexBuffer.hxx"
#include "VertexBuffer.hxx"

#include <imgui.h>

#include <memory>

namespace kyanite::engine::rendering {
	class Buffer;
	class RenderTarget;
	class GraphicsContext;
	class ImmediateGuiContext;
	class UploadContext;

	class Device {

	public:
		Device() = default;
		virtual ~Device() = default;
		virtual auto Shutdown() -> void = 0;

		// Creation work submission and synchronization
		virtual auto CreateGraphicsContext() -> std::unique_ptr<GraphicsContext> = 0;
		virtual auto CreateImGuiContext(ImGuiContext* context) -> 
			std::unique_ptr<ImmediateGuiContext> = 0;
		virtual auto CreateUploadContext() -> std::unique_ptr<UploadContext> = 0;
		virtual auto CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> = 0;
		virtual auto CreateCommandQueue(CommandListType type) -> std::shared_ptr <CommandQueue> = 0;
		virtual auto CreateCommandAllocator() -> std::shared_ptr<CommandAllocator> = 0;
		virtual auto CreateFence() -> std::shared_ptr<Fence> = 0;
		virtual auto CreateSwapchain() -> std::unique_ptr<Swapchain> = 0;

		// Creation of resources
		virtual auto CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> = 0;
		virtual auto CreateRenderTarget() -> std::shared_ptr<RenderTarget> = 0;
		virtual auto CreateMaterial(
			std::map<ShaderType, std::shared_ptr<Shader>> shaders,
			std::map<std::string, uint64_t> textures = {},
			std::map<std::string, float> floats = {},
			std::map<std::string, uint32_t> ints = {},
			std::map<std::string, bool> bools = {},
			std::map<std::string, std::array<float, 2>> vec2s = {},
			std::map<std::string, std::array<float, 3>> vec3s = {},
			std::map<std::string, std::array<float, 4>> vec4s = {}
		) -> std::shared_ptr<Material> = 0;
		virtual auto CompileShader(
			const std::string& shaderSource, 
			ShaderType type
		) -> std::shared_ptr<Shader> = 0;
		virtual auto CreateVertexBuffer(const void* data, uint64_t size) -> std::shared_ptr<VertexBuffer> = 0;
		virtual auto UpdateVertexBuffer(std::shared_ptr<VertexBuffer> buffer, const void* data, uint64_t size) -> void = 0;
		virtual auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> std::shared_ptr<IndexBuffer> = 0;
		virtual auto UpdateIndexBuffer(std::shared_ptr<IndexBuffer> buffer, std::vector<uint32_t> indices) -> void = 0;

		// Deleting resources
		virtual auto DestroyShader(uint64_t shaderHandle) -> void = 0;

	protected:
		std::shared_ptr<CommandQueue> _graphicsQueue;
		std::shared_ptr<CommandQueue> _computeQueue;
		std::shared_ptr<CommandQueue> _copyQueue;
		std::shared_ptr<CommandQueue> _directQueue;
	};
}