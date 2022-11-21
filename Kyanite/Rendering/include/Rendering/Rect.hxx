#pragma once

#include <cstdint>

namespace Renderer {
struct Rect {
    std::uint32_t Left;
    std::uint32_t Top;
    std::uint32_t Right;
    std::uint32_t Bottom; 
};
}