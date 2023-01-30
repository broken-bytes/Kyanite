#pragma once

#include "GraphicsCommandList.hxx"
#include <string>

namespace Renderer {
    class Allocator {
        public:
            Allocator(CommandType type, std::string name) {
            _type = type;
            _name = name;
        }

        virtual auto Reset() -> void = 0;
        [[nodiscard]] virtual auto Type() const -> CommandType {
            return _type;
        }

    protected:
        auto Name() -> std::string {
            return _name;
        }

        private:
        CommandType _type;
        std::string _name;
    };
}

