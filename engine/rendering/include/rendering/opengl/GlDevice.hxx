#pragma once

#include "../Device.hxx"
#include "../Shader.hxx"

namespace kyanite::engine::rendering::opengl {
	class GlDevice : public Device {
	public:
        GlDevice();
		virtual ~GlDevice();
        virtual auto Shutdown() -> void override;

        // Creation work submission and synchronization

        virtual auto CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> override;
        virtual auto CreateCommandQueue(CommandListType type) -> std::shared_ptr<CommandQueue> override;
        virtual auto CreateFence() -> std::shared_ptr<Fence> override;

        // Creation of resources
        virtual auto CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> override;
        virtual auto CreateRenderTarget() -> std::shared_ptr<RenderTarget> override;
        virtual auto CompileShader(const std::string& shaderSource, ShaderType type) -> uint64_t override;
        virtual auto CreateVertexBuffer(const void* data, uint64_t size) -> std::shared_ptr<VertexBuffer> override;
        virtual auto UpdateVertexBuffer(std::shared_ptr<VertexBuffer> buffer, const void* data, uint64_t size) -> void override;
        virtual auto CreateIndexBuffer(std::vector<uint32_t> indices) -> std::shared_ptr<IndexBuffer> override;
        virtual auto UpdateIndexBuffer(std::shared_ptr<IndexBuffer> buffer, std::vector<uint32_t> indices) -> void override;

        //Delete resources
        virtual auto DestroyShader(uint64_t shaderHandle) -> void override;
	};
}