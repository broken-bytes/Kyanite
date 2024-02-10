#include "rendering/IndexBuffer.hxx"
#include "rendering/PrimitiveTopology.hxx"
#include "rendering/VertexBuffer.hxx"
#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/opengl/GlIndexBuffer.hxx"
#include "rendering/opengl/GlVertexBuffer.hxx"

#include "glad/glad.h"

#include <stdexcept>

namespace kyanite::engine::rendering::opengl {
	GlCommandList::GlCommandList(CommandListType type) : CommandList(type) {

	}

	GlCommandList::~GlCommandList() {

	}

	auto GlCommandList::Begin() -> void {
		if (!_commands.empty()) {
			throw std::runtime_error("Cannot begin a commandlist that is already in recording state");
		}
	}

	auto GlCommandList::Close() -> void {
		// NOTE: This is a NOP for OpenGL
	}

	auto GlCommandList::Reset(CommandAllocator& allocator) -> void {
		_commands.clear();
	}

	auto GlCommandList::ClearRenderTarget(glm::vec4 color) -> void {
		_commands.push_back([color]() {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		});
	}

	auto GlCommandList::SetViewport(
		uint32_t x,
		uint32_t y,
		uint32_t width,
		uint32_t height,
		uint32_t minDepth,
		uint32_t maxDepth
	) -> void {
		_commands.push_back([x, y, width, height]() {
			glViewport(x, y, width, height);
		});
	}

	auto GlCommandList::SetScissorRect(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom) -> void {
		_commands.push_back([left, top, right, bottom]() {
			glScissor(left, top, right, bottom);
		});
	}

	auto GlCommandList::SetPrimitiveTopology(PrimitiveTopology topology) -> void {
		_commands.push_back([this, topology]() {
			switch (topology) {
			case PrimitiveTopology::TRIANGLE_LIST:
				_primitiveTopology = GL_TRIANGLES;
				break;
			case PrimitiveTopology::TRIANGLE_STRIP:
				_primitiveTopology = GL_TRIANGLE_STRIP;
				break;
			case PrimitiveTopology::LINE_LIST:
				_primitiveTopology = GL_LINES;
				break;
			case PrimitiveTopology::LINE_STRIP:
				_primitiveTopology = GL_LINE_STRIP;
				break;
			default:
				throw std::runtime_error("Unsupported primitive topology");
			}
		});
	}

	auto GlCommandList::BindVertexBuffer(VertexBuffer& vertexBuffer) -> void {
		_commands.push_back([this, &vertexBuffer]() {
			glBindBuffer(GL_ARRAY_BUFFER, reinterpret_cast<GlVertexBuffer&>(vertexBuffer).Id());
		});
	}

	auto GlCommandList::BindIndexBuffer(IndexBuffer& indexBuffer) -> void {
		_commands.push_back([this, &indexBuffer]() {
			glBindBuffer(GL_ARRAY_BUFFER, reinterpret_cast<GlIndexBuffer&>(indexBuffer).Id());
		});
	}

	auto GlCommandList::DrawIndexed(uint32_t numIndices, uint32_t startIndex, uint32_t startVertex) -> void {

	}
}