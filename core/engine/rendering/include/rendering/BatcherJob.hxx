#pragma once

#include "GraphicsContext.hxx"
#include "VertexBuffer.hxx"
#include "IndexBuffer.hxx"
#include "Material.hxx"
#include "CommandList.hxx"

#include <cstdint>
#include <functional>
#include <memory>
#include <thread>

namespace kyanite::engine::rendering {
	class BatcherJob {
		struct DrawCall {
			// Bit 0
			// Fullscreen
			// Bits 1-4 Viewport Index 
			// Bits 5-9 Layer Index
			// Bits 8-63 Sort Key
			uint64_t sortKey;
			std::shared_ptr<VertexBuffer> vertexBuffer;
			std::shared_ptr<IndexBuffer> indexBuffer;
			std::shared_ptr<Material> material;

			// Default constructor
			DrawCall() : sortKey(0), vertexBuffer(nullptr), indexBuffer(nullptr), material(nullptr) {}

			// Constructor
			DrawCall(
				uint64_t sortKey,
				std::shared_ptr<VertexBuffer>& vertexBuffer,
				std::shared_ptr<IndexBuffer>& indexBuffer,
				std::shared_ptr<Material>& material
			) {
				this->sortKey = sortKey;
				this->vertexBuffer = vertexBuffer;
				this->indexBuffer = indexBuffer;
				this->material = material;
			}

			// Copy constructor
			DrawCall(const DrawCall& other) {
				sortKey = other.sortKey;
				vertexBuffer = other.vertexBuffer;
				indexBuffer = other.indexBuffer;
				material = other.material;
			}

			auto operator<(const DrawCall& other) const -> bool {
				return sortKey < other.sortKey;
			}
		};
	public:
		/**
		* @brief Executes the job and sends the draw calls to the GPU
		*
		*/
		auto Execute(std::shared_ptr<CommandList>& commandList) -> void;

		/**
		* @brief Adds a draw call to the job
		*
		* @param sortKey The sort key of the draw call
		* @param drawCall the draw call to add
		*/
		auto SubmitDrawCall(uint64_t sortKey,
			std::shared_ptr<VertexBuffer>& vertexBuffer,
			std::shared_ptr<IndexBuffer>& indexBuffer,
			std::shared_ptr<Material>& material
		) -> void;

	private:
		std::vector<DrawCall> _drawCalls;
		std::thread _thread;
	};
}