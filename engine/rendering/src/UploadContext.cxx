#include "rendering/UploadContext.hxx"

#include <glad/glad.h>

namespace kyanite::engine::rendering {
	auto UploadContext::UpdateVertexBuffer(
		const std::shared_ptr<VertexBuffer>& buffer,
		const void* data,
		size_t size
	) -> void {
		// TODO: Move this to the commandlist
		buffer->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	auto UploadContext::UpdateIndexBuffer(
		const std::shared_ptr<IndexBuffer>& buffer,
		const void* data,
		size_t size
	) -> void {
		// TODO: Move this to the commandlist
		buffer->Bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
	}

	auto UploadContext::Begin() -> void {

	}

	auto UploadContext::Finish() -> void {

	}
}