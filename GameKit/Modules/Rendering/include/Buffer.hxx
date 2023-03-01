#pragma once

#include <cstdint>
#include <stddef.h>

namespace Rendering {
    
    class Buffer {

        virtual auto Size() const -> size_t = 0;
    };
}
