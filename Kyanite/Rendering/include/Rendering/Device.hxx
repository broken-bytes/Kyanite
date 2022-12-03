
#include <memory>
#include <vector>
#include "Allocator.hxx"
#include "Buffer.hxx"
#include "CommandQueue.hxx"
#include "DescriptorHandle.hxx"
#include "Fence.hxx"
#include "Frame.hxx"
#include "GraphicsCommandList.hxx"
#include "GraphicsPipelineState.hxx"
#include "GraphicsRootSignature.hxx"
#include "Heap.hxx"
#include "IndexBuffer.hxx"
#include "ReadbackBuffer.hxx"
#include "Renderer.hxx"
#include "RenderTarget.hxx"
#include "Shader.hxx"
#include "ShaderBinding.hxx"
#include "SwapChain.hxx"
#include "TextureBuffer.hxx"
#include "Vertex.hxx"
#include "UploadBuffer.hxx"
#include "VertexBuffer.hxx"
#include "VertexArrayObject.hxx"
#include <tuple>

#pragma once

namespace Renderer {
    class Device {
        public:
		Device(			
			void* window
		) {
			_window = window;
		}
		virtual ~Device() {};
        [[nodiscard]] virtual auto Name() -> const wchar_t* = 0;
        [[nodiscard]] virtual auto VRAM() -> const uint32_t = 0;  
        [[nodiscard]] virtual auto CreateSwapChain(
			std::shared_ptr<CommandQueue> queue,
			uint8_t frameCount,
			uint32_t width,
			uint32_t height
		) ->
			std::shared_ptr<SwapChain> = 0;
		[[nodiscard]] virtual auto CreateCommandAllocator(CommandType type) -> std::shared_ptr<Allocator> = 0;
        [[nodiscard]] virtual auto CreateCommandQueue(CommandType type) -> std::shared_ptr<CommandQueue> = 0;
		[[nodiscard]] virtual auto CreateCommandList(
			CommandType type,
			std::shared_ptr<Allocator> allocator,
			const char* name = "CommandList"
		)->std::shared_ptr<GraphicsCommandList> = 0;
        [[nodiscard]] virtual auto CreateRenderTargetHeap(size_t count) -> std::shared_ptr<Heap> = 0;
		[[nodiscard]] virtual auto CreateConstantBufferHeap(size_t count) -> std::shared_ptr<Heap> = 0;
		[[nodiscard]] virtual auto CreateShaderResourceHeap(size_t count) -> std::shared_ptr<Heap> = 0;
		[[nodiscard]] virtual auto CreateUnorderedAccessHeap(size_t count) -> std::shared_ptr<Heap> = 0;
		[[nodiscard]] virtual auto CreateDepthStencilHeap(size_t count) -> std::shared_ptr<Heap> = 0;
		[[nodiscard]] virtual auto CreateSamplerHeap(size_t count) -> std::shared_ptr<Heap> = 0;
        [[nodiscard]] virtual auto CreateVertexBuffer(std::vector<Vertex> vertices) -> std::shared_ptr<Renderer::Buffer> = 0;
		[[nodiscard]] virtual auto CreateIndexBuffer(std::vector<uint32_t> indices)->std::shared_ptr<Renderer::Buffer> = 0;
		[[nodiscard]] virtual auto CreateUploadBuffer(size_t sizeInBytes) -> std::shared_ptr<Renderer::UploadBuffer> = 0;
        [[nodiscard]] virtual auto CreateVertexArrayObject(Renderer::VertexBuffer* vertexBuffer, Renderer::IndexBuffer* indexBuffer) -> std::shared_ptr<VertexArrayObject> = 0;
        [[nodiscard]] virtual auto CreateCommandQueue() -> std::shared_ptr<CommandQueue> = 0;
        [[nodiscard]] virtual auto CreateTextureBuffer(
			uint16_t width,
			uint16_t height,
			uint8_t channels,
			uint8_t mipLevels,
			TextureFormat format,
			const char* name = "TextureBuffer"
		)->std::shared_ptr<Renderer::TextureBuffer> = 0;

		[[nodiscard]] virtual auto CreateReadbackBuffer(size_t sizeInBytes) -> std::shared_ptr<ReadbackBuffer> = 0;

		[[nodiscard]] virtual auto CreateConstantBuffer(
			void* data,
			size_t size,
			const char* name = "ConstantBuffer"
		)->std::shared_ptr<Buffer> = 0;

		[[nodiscard]] virtual auto CreateGraphicsRootSignature(
			GraphicsRootSignatureDescription& rootDesc
		)-> std::shared_ptr<GraphicsRootSignature> = 0;

		[[nodiscard]] virtual auto CreatePipelineState(
			std::shared_ptr <GraphicsRootSignature> rootSignature,
			std::vector<GraphicsPipelineInputElement> inputLayout,
			std::shared_ptr<GraphicsShaderBinding> shaderBinding,
			TextureFormat format,
			bool depth = true,
			bool stencil = true,
			GraphicsPipelineStateTopology topology = GraphicsPipelineStateTopology::POLYGON
		)->std::shared_ptr<GraphicsPipelineState> = 0;

		[[nodiscard]] virtual auto CreateDepthBuffer(
			glm::vec2 dimensions
		)->std::shared_ptr<Buffer> = 0;

		virtual auto CreateDepthStencilView(
			std::shared_ptr<DescriptorHandle> handle,
			std::shared_ptr<Buffer> buffer
		) -> void = 0;
        [[nodiscard]] virtual auto CreateFrame(std::shared_ptr<Allocator> allocator, std::shared_ptr<RenderTarget> renderTarget) -> std::shared_ptr<Frame> = 0;
		[[nodiscard]] virtual auto CreateFence(std::uint64_t fenceValue) -> std::shared_ptr<Fence> = 0;
		[[nodiscard]] virtual auto CreateRenderTarget(std::shared_ptr<TextureBuffer> texture) -> std::shared_ptr<RenderTarget> = 0;
		[[nodiscard]] virtual auto CreateRenderTargetView(std::shared_ptr<DescriptorHandle> handle, std::shared_ptr<RenderTarget> target)->std::shared_ptr<RenderTarget> = 0;
		virtual auto CreateShaderResourceView(std::shared_ptr<TextureBuffer> buffer, std::shared_ptr<DescriptorHandle> handle) -> void = 0;
		virtual auto CreateConstantBufferView(std::shared_ptr<Heap> heap, std::shared_ptr<UploadBuffer> buffer, std::shared_ptr<DescriptorHandle> cpuHandle) -> void = 0;
		virtual auto CreateMipMaps(std::shared_ptr<TextureBuffer> texture, std::shared_ptr<GraphicsCommandList> commandList, uint16_t width, uint16_t height, uint8_t levels) -> void = 0;
        [[nodiscard]] virtual auto CompileShader(std::string code) ->std::shared_ptr<GraphicsShaderBinding> = 0;
        virtual auto Flush() -> void = 0;

	protected:
		void* _window;
    };
}

