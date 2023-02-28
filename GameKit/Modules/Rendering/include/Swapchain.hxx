#pragma once

#include "Common/PCH.hxx"
#include "Resource.hxx"

namespace Rendering {

    class Swapchain {

        public:
        Swapchain(Window window);
        virtual auto CurrentBackBuffer() -> Resource = 0;
        virtual auto BackBuffer(uint8_t index) -> Resource = 0;
        virtual auto Present() -> void = 0;
        virtual auto Resize(uint8_t numBackBuffers, uint32_t width, uint32_t height) -> void = 0;
    };
}