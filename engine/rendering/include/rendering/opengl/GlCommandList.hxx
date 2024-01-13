#pragma once

#include "../CommandList.hxx"
#include "../CommandListType.hxx"
#include "../IndexBuffer.hxx"
#include "../VertexBuffer.hxx"

#include <functional>
#include <vector>

namespace rendering::opengl {
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
		virtual auto BindVertexBuffer(VertexBuffer& vertexBuffer) -> void override;
		virtual auto BindIndexBuffer(IndexBuffer& indexBuffer) -> void override;
		virtual auto DrawIndexed(uint32_t numIndices, uint32_t startIndex, uint32_t startVertex) -> void override;

	private:
		std::vector<std::function<void()>> _commands;
	};
}