#pragma once

#include "Context.hxx"

namespace kyanite::engine::rendering {
	class UploadContext : public Context {
	public:
		UploadContext(
			const std::shared_ptr<Device>& device,
			std::shared_ptr<CommandQueue> queue
		) : Context(CommandListType::Copy, device, queue) { }
		auto UpdateVertexBuffer(
			const std::shared_ptr<VertexBuffer>& buffer,
			const void* data,
			size_t size
		) -> void;
		auto UpdateIndexBuffer(
			const std::shared_ptr<IndexBuffer>& buffer,
			const void* data,
			size_t size
		) -> void;
		virtual ~UploadContext() = default;
		virtual auto Begin() -> void override;
		virtual auto Finish() -> void override;
	};
}