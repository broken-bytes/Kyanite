#pragma once

#include "CommandList.hxx"
#include "CommandListType.hxx"
#include "Fence.hxx"

#include <vector>

namespace rendering {
	class CommandQueue {
	public:
		CommandQueue(CommandListType type) : _type(type) {}
		virtual ~CommandQueue() = default;
		virtual void Execute(const std::vector<CommandList>& commandLists) = 0;
		virtual void Signal(Fence& fence, uint64_t value) = 0;

		auto Type() const { return _type; }

	private:
		CommandListType _type;
	};
}