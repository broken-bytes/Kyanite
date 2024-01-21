#pragma once 

#include "../CommandQueue.hxx"

namespace kyanite::engine::rendering::opengl {
	class GlCommandQueue : public CommandQueue {
	public:
		GlCommandQueue(CommandListType type);
		~GlCommandQueue() = default;

		virtual auto Execute(const std::vector<CommandList>& commandLists) -> void override;
		virtual auto Signal(Fence& fence, uint64_t value) -> void override;
	};
}