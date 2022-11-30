#pragma once
#include "glm/glm.hpp"
#include "DescriptorHandle.hxx"
#include "GraphicsCommandList.hxx"
#include "Renderer.hxx"
#include "RenderTarget.hxx"
#include "Viewport.hxx"
#include <d3d12.h>
#include <wrl.h>

namespace Renderer {
	class D3D12GraphicsCommandList : public GraphicsCommandList {
	public:
		D3D12GraphicsCommandList(CommandType type, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList5> commandList);
		virtual auto Close() -> void override;
        virtual auto Reset(std::shared_ptr<Allocator> allocator, std::shared_ptr<GraphicsPipelineState> pipelineState) -> void override;
        virtual auto ClearRenderTarget(std::shared_ptr<DescriptorHandle> target, glm::vec4 color) -> void override;
		virtual auto ClearDepthTarget(std::shared_ptr<DescriptorHandle> target, float depth) -> void override;
		virtual auto SetPipelineState(std::shared_ptr<GraphicsPipelineState> state) -> void override;
		virtual auto SetGraphicsRootSignature(std::shared_ptr<GraphicsRootSignature> signature) -> void override;
		virtual auto SetDescriptorHeaps(std::vector<std::shared_ptr<Heap>> heaps) -> void override;
		virtual auto SetGraphicsRootConstantBuffer(std::shared_ptr<UploadBuffer> heap, void* data, size_t sizeInBytes, uint8_t* address) -> void override;
		virtual auto SetGraphicsRootDescriptorTable(
			std::uint32_t index,
			std::shared_ptr<DescriptorHandle> handle
		) -> void override;
		virtual auto SetGraphicsRootConstants(std::uint64_t index, std::size_t count, void* data, std::size_t offset) -> void override;
		virtual auto SetTopology(GraphicsPipelineStateTopology topology) -> void override;
		virtual auto SetViewport(Viewport viewport) -> void override;
		virtual auto SetScissorRect(Rect rect) -> void override;
		virtual auto SetRenderTarget(
			std::shared_ptr<DescriptorHandle> rtvHandle,
			std::shared_ptr<DescriptorHandle> dsvHandle
		) -> void override;
		virtual auto SetVertexBuffer(
			std::shared_ptr<Buffer> buffer
		) -> void override;
		virtual auto SetIndexBuffer(
			std::shared_ptr<Buffer> buffer
		) -> void override;
        virtual auto Draw(
            std::uint32_t vertexCount,
			std::uint32_t instanceCount = 1,
			std::uint32_t startVertex = 0,
			std::uint32_t startInstance = 0) -> void override;
		virtual auto DrawInstanced(
			std::uint32_t indexCount,
			std::uint32_t instanceCount = 1,
			std::uint32_t startIndex = 0,
			std::uint32_t startVertex = 0,
			std::uint32_t startInstance = 0
		) -> void override;
		virtual auto Transition(std::shared_ptr<Resource> resource, ResourceState from, ResourceState to) -> void override;
		virtual auto Transition(std::shared_ptr<RenderTarget> resource, ResourceState from, ResourceState to) -> void override;
		virtual auto Transition(Resource* resource, ResourceState from, ResourceState to) -> void override;
		virtual auto Transition(void* resource, ResourceState from, ResourceState to) -> void override;
		virtual auto Transition(std::shared_ptr<Renderer::Buffer> buffer, ResourceState from, ResourceState to) -> void override;
		virtual auto Transition(std::shared_ptr<Renderer::TextureBuffer> buffer, ResourceState from, ResourceState to) -> void override;

		virtual auto UpdateSubresources(std::shared_ptr<Buffer> dst, std::shared_ptr<UploadBuffer> src, void* data, size_t rowPitch, size_t slicePitch) -> void override;
		virtual auto UpdateSubresources(std::shared_ptr<TextureBuffer> dst, std::shared_ptr<UploadBuffer> src, void* data, size_t rowPitch, size_t slicePitch) -> void override;

		virtual auto Copy(std::shared_ptr<RenderTarget> from, std::shared_ptr<ReadbackBuffer> to) -> void override;

		auto Native() const ->ID3D12GraphicsCommandList5*;

	private:
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList5> _commandList;
	};
}