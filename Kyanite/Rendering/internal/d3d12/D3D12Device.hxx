
#pragma once

#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <memory>
#include <vector>
#include <wrl.h>
#include "Allocator.hxx"
#include "Buffer.hxx"
#include "CommandQueue.hxx"
#include "DescriptorHandle.hxx"
#include "Device.hxx"
#include "Fence.hxx"
#include "Frame.hxx"
#include "GraphicsCommandList.hxx"
#include "GraphicsPipelineState.hxx"
#include "GraphicsRootSignature.hxx"
#include "Heap.hxx"
#include "IndexBuffer.hxx"
#include "Renderer.hxx"
#include "Shader.hxx"
#include "SwapChain.hxx"
#include "TextureBuffer.hxx"
#include "UploadBuffer.hxx"
#include "Vertex.hxx"
#include "VertexBuffer.hxx"
#include "VertexArrayObject.hxx"


namespace Renderer {
    class D3D12Device: public Device {
        public:
			D3D12Device(
				HWND window
			);
		virtual ~D3D12Device();
        [[nodiscard]] virtual auto Name() -> const wchar_t*;
        [[nodiscard]] virtual auto VRAM() -> const uint32_t;  
        [[nodiscard]] virtual auto CreateSwapChain(
			std::shared_ptr<CommandQueue> queue,
			uint8_t frameCount,
			uint32_t width,
			uint32_t height
		) ->
			std::shared_ptr<SwapChain>;
		[[nodiscard]] virtual auto CreateCommandAllocator(CommandType type) -> std::shared_ptr<Allocator>;
        [[nodiscard]] virtual auto CreateCommandQueue(CommandType type) -> std::shared_ptr<CommandQueue>;
		[[nodiscard]] virtual auto CreateCommandList(
			CommandType type,
			std::shared_ptr<Allocator> allocator,
			const char* name = "CommandList"
		)->std::shared_ptr<GraphicsCommandList>;
        [[nodiscard]] virtual auto CreateRenderTargetHeap(size_t count) -> std::shared_ptr<Heap>;
		[[nodiscard]] virtual auto CreateConstantBufferHeap(size_t count) -> std::shared_ptr<Heap>;
		[[nodiscard]] virtual auto CreateShaderResourceHeap(size_t count) -> std::shared_ptr<Heap>;
		[[nodiscard]] virtual auto CreateUnorderedAccessHeap(size_t count) -> std::shared_ptr<Heap>;
		[[nodiscard]] virtual auto CreateDepthStencilHeap(size_t count) -> std::shared_ptr<Heap>;
		[[nodiscard]] virtual auto CreateSamplerHeap(size_t count) -> std::shared_ptr<Heap>;
        [[nodiscard]] virtual auto CreateVertexBuffer(std::vector<Vertex> vertices) -> std::shared_ptr<Renderer::Buffer>;
		[[nodiscard]] virtual auto CreateIndexBuffer(std::vector<uint32_t> indices)->std::shared_ptr<Renderer::Buffer>;
		[[nodiscard]] virtual auto CreateUploadBuffer(size_t sizeInBytes)->std::shared_ptr<Renderer::UploadBuffer>;
        [[nodiscard]] virtual auto CreateVertexArrayObject(Renderer::VertexBuffer* vertexBuffer, Renderer::IndexBuffer* indexBuffer) -> std::shared_ptr<VertexArrayObject>;
        [[nodiscard]] virtual auto CreateCommandQueue() -> std::shared_ptr<CommandQueue>;
        [[nodiscard]] virtual auto CreateTextureBuffer(
			uint16_t width,
			uint16_t height,
			uint8_t channels,
			uint8_t mipLevels,
			TextureFormat format,
			const char* name = "TextureBuffer"
		)->std::shared_ptr<Renderer::TextureBuffer>;

		[[nodiscard]] virtual auto CreateConstantBuffer(
			void* data,
			size_t size,
			const char* name = "ConstantBuffer"
		)->std::shared_ptr<Buffer>;

		[[nodiscard]] virtual auto CreateGraphicsRootSignature(
			GraphicsRootSignatureDescription& rootDesc
		)-> std::shared_ptr<GraphicsRootSignature>;

		[[nodiscard]] virtual auto CreatePipelineState(
			std::shared_ptr<GraphicsRootSignature> rootSignature,
			std::vector<GraphicsPipelineInputElement> inputLayout,
			std::shared_ptr < GraphicsShaderBinding> shaderBinding,
			bool depth = true,
			bool stencil = true,
			GraphicsPipelineStateTopology topology = GraphicsPipelineStateTopology::POLYGON
		)->std::shared_ptr<GraphicsPipelineState>;

		[[nodiscard]] virtual auto CreateDepthBuffer(
			glm::vec2 dimensions
		)->std::shared_ptr<Buffer>;

		virtual auto CreateDepthStencilView(
			std::shared_ptr<Heap> heap,
			std::shared_ptr<Buffer> buffer
		) -> void;
        [[nodiscard]] virtual auto CreateFrame(std::shared_ptr<Allocator> allocator, std::shared_ptr<RenderTarget> renderTarget) -> std::shared_ptr<Frame>;
		[[nodiscard]] virtual auto CreateFence(std::uint64_t fenceValue) -> std::shared_ptr<Fence>;
		[[nodiscard]] virtual auto CreateRenderTarget(std::shared_ptr<DescriptorHandle> handle, std::shared_ptr<RenderTarget> target)->std::shared_ptr<RenderTarget>;
		virtual auto CreateShaderResourceView(std::shared_ptr<TextureBuffer> buffer, std::shared_ptr<DescriptorHandle> handle) -> void;
		virtual auto CreateConstantBufferView(std::shared_ptr<Heap> heap, std::shared_ptr<UploadBuffer> buffer, std::shared_ptr<DescriptorHandle> cpuHandle) -> void;

		virtual auto CreateMipMaps(std::shared_ptr<TextureBuffer> texture, std::shared_ptr<GraphicsCommandList> commandList, uint16_t width, uint16_t height, uint8_t levels) -> void;
        [[nodiscard]] virtual auto CompileShader(std::string code) ->std::shared_ptr<GraphicsShaderBinding>;
        virtual auto Flush() -> void;
		[[nodiscard]] virtual auto Native() -> ID3D12Device4*;

	private:
		Microsoft::WRL::ComPtr<ID3D12Device4> _device;
    };
}

