#include "rendering/opengl/GlCommandQueue.hxx"
#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/opengl/GlFence.hxx"

namespace rendering::opengl {
	GlCommandQueue::GlCommandQueue(CommandListType type) : CommandQueue(type) {

	}

	auto GlCommandQueue::Execute(const std::vector<CommandList>& commandLists) -> void {
		for(auto& commandList : commandLists) {
			for(auto& command : reinterpret_cast<const GlCommandList*>(&commandList)->_commands) {
				command();
			}
		}
	}

	auto GlCommandQueue::Signal(Fence& fence, uint64_t value) -> void {
		fence.SetOnCompletion(value, nullptr);
	}
}