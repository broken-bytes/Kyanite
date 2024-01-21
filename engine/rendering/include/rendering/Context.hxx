#pragma once

#include "CommandList.hxx"
#include "CommandListType.hxx"
#include "Device.hxx"

#include <memory>

namespace kyanite::engine::rendering {
    class Context {
    public:
        explicit Context(CommandListType type, std::shared_ptr<Device> device) : _type(type), _device(device) {

        }

        auto Init() -> void {
            _commandList = _device->CreateCommandList(_type);
        }

        auto Deinit() -> void {
            delete _commandList;
        }

        virtual ~Context() = default;

    protected:
        auto GetCommandList() -> const CommandList* const {
            return _commandList;
        }

    private:
        CommandListType _type;
        CommandList* _commandList;
        std::shared_ptr<Device> _device;
    };
}
