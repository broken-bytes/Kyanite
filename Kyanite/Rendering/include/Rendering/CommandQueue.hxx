
#include <string>
#include <vector>

#include "GraphicsCommandList.hxx"
#include "Fence.hxx"

#ifndef _COMMANDQUEUE_HXX_
#define _COMMANDQUEUE_HXX_

namespace Renderer {
    class CommandQueue {
        public:
        friend class Device;
        CommandQueue(CommandType type, std::string name) {
            _name = name;
        }
        virtual auto ExecuteCommandLists(std::vector<std::shared_ptr<GraphicsCommandList>> lists) -> void = 0;
        [[nodiscard]] virtual auto Signal(std::shared_ptr<Fence> fence, uint64_t value) -> uint8_t = 0;

    protected:
        auto Name() -> std::string {
            return _name;
        }

    private:
        std::string _name;
    };
}

#endif