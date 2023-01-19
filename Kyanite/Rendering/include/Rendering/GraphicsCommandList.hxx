#pragma once

#include <cstdint>
#include <memory>
#include "ReadbackBuffer.hxx"
#include "glm/glm.hpp"
#include <vector>
#include "DescriptorHandle.hxx"
#include "GraphicsPipelineState.hxx"
#include "Renderer.hxx"
#include "RenderTarget.hxx"
#include "Resource.hxx"
#include "UploadBuffer.hxx"
#include "TextureBuffer.hxx"


namespace Renderer {
#pragma region FORWARD_INCLUDES
	struct ViewPort;
	struct Rect;
	class VertexBuffer;
	class Buffer;
	class IndexBuffer;
	class GraphicsRootSignature;
	struct Viewport;
	class Heap;
	class Allocator;
#pragma endregion

	enum CommandType {
        DIRECT	= 0,
		BUNDLE  = 1,
       	COMPUTE	= 2,
        COPY	= 3,
    };

    class GraphicsCommandList {
#pragma region PUBLIC
        public:
		friend class Device;

		GraphicsCommandList(CommandType type) {
			_type = type;
		}

		auto Type() const -> CommandType {
			return  _type;
		}

        virtual auto Close() -> void = 0;
        virtual auto Reset(std::shared_ptr<Allocator> allocator, std::shared_ptr<GraphicsPipelineState> pipelineState) -> void = 0;
        virtual auto ClearRenderTarget(std::shared_ptr<CPUDescriptorHandle> target, glm::vec4 color) -> void = 0;
		virtual auto ClearDepthTarget(std::shared_ptr<CPUDescriptorHandle> target, float depth = 1.0) -> void = 0;
		virtual auto SetPipelineState(std::shared_ptr<GraphicsPipelineState> state) -> void = 0;
		virtual auto SetGraphicsRootConstantBuffer(std::shared_ptr<UploadBuffer> heap, void* data, size_t sizeInBytes, uint8_t* address) -> void = 0;
		virtual auto SetGraphicsRootSignature(std::shared_ptr<GraphicsRootSignature> signature) -> void = 0;
		virtual auto SetDescriptorHeaps(std::vector<std::shared_ptr<Heap>> heaps) -> void = 0;
		virtual auto SetGraphicsRootDescriptorTable(
			std::uint32_t index,
			std::shared_ptr<GPUDescriptorHandle> handle
		) -> void = 0;
		virtual auto SetGraphicsRootConstants(std::uint64_t index, std::size_t count, void* data, std::size_t offset) -> void = 0;
		virtual auto SetTopology(GraphicsPipelineStateTopology topology) -> void = 0;
		virtual auto SetViewport(Viewport viewport) -> void = 0;
		virtual auto SetScissorRect(Rect rect) -> void = 0;
		virtual auto SetRenderTarget(
			std::shared_ptr<CPUDescriptorHandle> rtvHandle,
			std::shared_ptr<CPUDescriptorHandle> dsvHandle
		) -> void = 0;
		virtual auto SetVertexBuffer(
			std::shared_ptr<Buffer> buffer
		) -> void = 0;
		virtual auto SetIndexBuffer(
			std::shared_ptr<Buffer> buffer
		) -> void = 0;
        virtual auto Draw(
            std::uint32_t vertexCount,
			std::uint32_t instanceCount = 1,
			std::uint32_t startVertex = 0,
			std::uint32_t startInstance = 0) -> void = 0;
		virtual auto DrawInstanced(
			std::uint32_t indexCount,
			std::uint32_t instanceCount = 1,
			std::uint32_t startIndex = 0,
			std::uint32_t startVertex = 0,
			std::uint32_t startInstance = 0
		) -> void = 0;
		virtual auto Transition(std::shared_ptr<Resource> resource, ResourceState from, ResourceState to) -> void = 0;
		virtual auto Transition(std::shared_ptr<RenderTarget> resource, ResourceState from, ResourceState to) -> void = 0;
		virtual auto Transition(std::shared_ptr<Renderer::Buffer> buffer, ResourceState from, ResourceState to) -> void = 0;
		virtual auto Transition(Resource* resource, ResourceState from, ResourceState to) -> void = 0;
		virtual auto Transition(void* resource, ResourceState from, ResourceState to) -> void = 0;
		virtual auto Transition(std::shared_ptr<Renderer::TextureBuffer> buffer, ResourceState from, ResourceState to) -> void = 0;

		virtual auto UpdateSubresources(std::shared_ptr<Buffer> dst, std::shared_ptr<UploadBuffer> src, void* data, size_t rowPitch, size_t slicePitch) -> void = 0;
		virtual auto UpdateSubresources(std::shared_ptr<TextureBuffer> dst, std::shared_ptr<UploadBuffer> src, void* data, size_t rowPitch, size_t slicePitch) -> void = 0;

		virtual auto Copy(std::shared_ptr<RenderTarget> from, std::shared_ptr<ReadbackBuffer> to) -> void = 0;
		virtual auto Copy(uint32_t startX, uint32_t startY, uint32_t width, uint32_t height, std::shared_ptr<TextureBuffer> from, std::shared_ptr<ReadbackBuffer> to) -> void = 0;


#pragma endregion
        private:
		CommandType _type;
    };
}
