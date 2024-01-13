#pragma once

#include "CommandAllocator.hxx"
#include "CommandListType.hxx"
#include "Vertex.hxx"
#include "IndexBuffer.hxx"
#include "VertexBuffer.hxx"
#include "core/Mesh.hxx"
#include "core/Material.hxx"

#include <glm/glm.hpp>

namespace rendering {
	class CommandList {
	public:
		CommandList(CommandListType type) : _type(type) {};
		virtual ~CommandList() = default;
		virtual auto Begin() -> void = 0;
		virtual auto Close() -> void = 0;
		virtual auto Reset(CommandAllocator& allocator) -> void = 0;
		virtual auto ClearRenderTarget(glm::vec4 color) -> void = 0;
		virtual auto BindVertexBuffer(VertexBuffer& vertexBuffer) -> void = 0;
		virtual auto BindIndexBuffer(IndexBuffer& indexBuffer) -> void = 0;
		virtual auto DrawIndexed(uint32_t numIndices, uint32_t startIndex, uint32_t startVertex) -> void = 0;

		auto Type() const -> CommandListType { return _type; }

	private:
		CommandListType _type;
	};
}