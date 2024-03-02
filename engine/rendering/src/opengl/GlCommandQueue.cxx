#include "rendering/opengl/GlCommandQueue.hxx"
#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/opengl/GlFence.hxx"

#include <future>

namespace kyanite::engine::rendering::opengl {
	GlCommandQueue::GlCommandQueue(CommandListType type) : CommandQueue(type) {

	}

	auto GlCommandQueue::Execute(const std::vector<std::shared_ptr<CommandList>>& commandLists) -> void {
		for (auto& commandList : commandLists) {
			for (auto& command : std::static_pointer_cast<GlCommandList>(commandList)->_commands) {
				command();
			}
		}
	}

	auto GlCommandQueue::Signal(Fence& fence, uint64_t value) -> void {
		fence.SetOnCompletion(value, nullptr);
	}
}