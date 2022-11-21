#pragma once

#include <cstdint>
#include <memory>
#include <CommandQueue.hxx>
#include <RenderTarget.hxx>

namespace Renderer {
    class SwapChain {
        public:
        friend class Device;
        SwapChain(
            std::shared_ptr<CommandQueue> queue,
            void* window,
            std::uint8_t frameCount,
            std::uint32_t width,
            std::uint32_t height
        ) {
            
        }
        virtual auto Swap() -> void = 0;
        [[nodiscard]] virtual auto CurrentBackBufferIndex() -> const std::uint64_t = 0;
        [[nodiscard]] virtual auto GetBuffer(std::uint8_t frameIndex)->std::shared_ptr<RenderTarget> = 0;
        private:
    };
}
