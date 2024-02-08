#pragma once

#include "CommandList.hxx"
#include "CommandListType.hxx"
#include "CommandQueue.hxx"
#include "Fence.hxx"
#include "Shader.hxx"

#include <memory>

namespace kyanite::engine::rendering {
	class Buffer;
	class RenderTarget;

	class Device {

	public:
		Device() = default;
		virtual ~Device() = default;
		virtual auto Shutdown() -> void = 0;

		// Creation work submission and synchronization

		virtual auto CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> = 0;
		virtual auto CreateCommandQueue(CommandListType type) -> std::shared_ptr < CommandQueue> = 0;
		virtual auto CreateFence() -> std::shared_ptr<Fence> = 0;

		// Creation of resources
		virtual auto CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> = 0;
		virtual auto CreateRenderTarget() -> std::shared_ptr<RenderTarget> = 0;
		virtual auto CompileShader(const std::string& shaderSource, ShaderType type) -> uint64_t = 0;

		// Deleting resources
		virtual auto DestroyShader(uint64_t shaderHandle) -> void = 0;
	};
}