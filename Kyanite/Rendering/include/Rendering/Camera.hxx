#pragma once

#include <array>
#include <cstdint>
#include "RenderTarget.hxx"

namespace Renderer {
    enum class CameraClearMode {
        DONT_CLEAR,
        COLOR_CLEAR,
        DEPTH_CLEAR
    };

    struct Camera {
        float fov;
        CameraClearMode Clear;
        uint16_t LayerMask;
        RenderTarget Target;
        uint8_t Index;
        std::array<float, 2> ClippingPlanes;
        std::array<float, 4> Viewport;
    };
}
