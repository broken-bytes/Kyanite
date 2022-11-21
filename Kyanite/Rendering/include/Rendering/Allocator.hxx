#pragma once

#include "GraphicsCommandList.hxx"

namespace Renderer {
    class Allocator {
        public:
        Allocator(CommandType type) {
            _type = type;
        }

        virtual auto Reset() -> void = 0;
        [[nodiscard]] virtual auto Type() const -> CommandType {
            return _type;
        }
        private:
        CommandType _type;
    };
}

