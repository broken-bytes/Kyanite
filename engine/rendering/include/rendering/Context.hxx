#pragma once

#include "CommandQueue.hxx"
#include "CommandList.hxx"
#include "CommandListType.hxx"

#include <memory>

namespace kyanite::engine::rendering {
    // Forward declaration
    class Device;
    class Context {
    public:
        explicit Context(
            CommandListType type, 
            const std::shared_ptr<Device>& device,
            std::shared_ptr<CommandQueue> queue
        );
        virtual ~Context() = default;
        virtual auto Begin() -> void = 0;
        virtual auto Finish() -> void = 0;

    protected:
        std::shared_ptr<CommandQueue> _commandQueue;
        std::shared_ptr<CommandAllocator> _commandAllocator;
        std::shared_ptr<CommandList> _commandList;

    private:
        CommandListType _type;
        std::shared_ptr<Device> _device;
    };
}
