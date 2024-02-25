#pragma once

#include "CommandList.hxx"
#include "CommandListType.hxx"

#include <memory>

namespace kyanite::engine::rendering {
    // Forward declaration
    class Device;
    class Context {
    public:
        explicit Context(CommandListType type, const std::shared_ptr<Device>& device);
        virtual ~Context() = default;

    protected:
        std::shared_ptr<CommandList> _commandList;

    private:
        CommandListType _type;
        std::shared_ptr<Device> _device;
    };
}
