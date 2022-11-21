#pragma once
#include <memory>

#include "IndexBuffer.hxx"
#include "VertexBuffer.hxx"

namespace Renderer {
	class VertexArrayObject {
	public:
		std::uint64_t Index;
		std::shared_ptr<Renderer::Buffer> VertexBuffer;
		std::shared_ptr<Renderer::Buffer> IndexBuffer;

		VertexArrayObject(std::uint64_t index,
			std::shared_ptr<Renderer::Buffer> vertexBuffer,
			std::shared_ptr<Renderer::Buffer> indexBuffer
		) {
			this->Index = index;
			this->VertexBuffer = vertexBuffer;
			this->IndexBuffer = indexBuffer;
		}

	};
}