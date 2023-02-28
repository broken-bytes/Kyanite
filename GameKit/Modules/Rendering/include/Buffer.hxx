#pragma once

#include <cstdint>

namespace Rendering {
    
    class Buffer {

        virtual auto Size() const -> size_t = 0;
    };
}