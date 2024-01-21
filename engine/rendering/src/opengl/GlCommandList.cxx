#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/IndexBuffer.hxx"
#include "rendering/VertexBuffer.hxx"
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
		if(!_commands.empty()) {
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
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
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