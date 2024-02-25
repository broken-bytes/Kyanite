#pragma once

#include "../CommandList.hxx"
#include "../CommandListType.hxx"
#include "../IndexBuffer.hxx"
#include "../PrimitiveTopology.hxx"
#include "../VertexBuffer.hxx"

#include <glad/glad.h>

#include <functional>
#include <vector>

namespace kyanite::engine::rendering::opengl {
	class GlCommandList: public CommandList {

	// Used by GlCommandQueue to access the _commands vector
	friend class GlCommandQueue;

	public:
		GlCommandList(CommandListType type);
		~GlCommandList();
		virtual auto Begin() -> void override;
		virtual auto Close() -> void override;
		virtual auto Reset(CommandAllocator& allocator) -> void override;
		virtual auto ClearRenderTarget(glm::vec4 color) -> void override;
		virtual auto SetViewport(
			uint32_t x, 
			uint32_t y, 
			uint32_t width, 
			uint32_t height, 
			uint32_t minDepth,
			uint32_t maxDepth
		) -> void override;
		virtual auto SetScissorRect(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom) -> void override;
		virtual auto SetPrimitiveTopology(PrimitiveTopology topology) -> void override;
		virtual auto BindVertexBuffer(VertexBuffer& vertexBuffer) -> void override;
		virtual auto BindIndexBuffer(IndexBuffer& indexBuffer) -> void override;
		virtual auto DrawIndexed(uint32_t numIndices, uint32_t startIndex, uint32_t startVertex) -> void override;

	private:
		std::vector<std::function<void()>> _commands;
		GLenum _primitiveTopology;
	};
}