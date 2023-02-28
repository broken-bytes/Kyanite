#pragma once

#include "CommandList.hxx"
#include "Fence.hxx"

#include "Common/PCH.hxx"

namespace Rendering {

    class CommandQueue {

        public:
        virtual auto ExecuteCommandLists(std::vector<CommandList> lists) -> void = 0;
        virtual auto Signal(Fence fence, uint64_t value) -> uint64_t = 0;
    };
}