#pragma once 

#include <cstdint>
#include <memory>

#include "Allocator.hxx"
#include "RenderTarget.hxx"

namespace Renderer {
    class SwapChain;
    class Device;
    class Frame {
        public:
        friend class Device;
        Frame(
            std::shared_ptr<Renderer::Allocator> allocator,
            std::shared_ptr<Renderer::RenderTarget> renderTarget
        ) {
            this->_allocator = allocator;
            this->_renderTarget = renderTarget;
        }

        virtual ~Frame() {

        }

        [[nodiscard]] virtual auto FenceValue() const -> const std::uint64_t {
            return _fenceValue;
        }

		[[nodiscard]] virtual auto Allocator() const -> std::shared_ptr<Renderer::Allocator> {
            return _allocator;
        }

        [[nodiscard]] virtual auto RenderTarget() const -> std::shared_ptr<Renderer::RenderTarget> {
            return _renderTarget;
        }

        virtual auto ResetRenderTarget() -> void {
            _renderTarget->Reset();
        }

        virtual auto SetRenderTarget(std::shared_ptr<Renderer::RenderTarget> renderTarget) -> void {
            _renderTarget = renderTarget;
        }

		[[nodiscard]] auto FenceValue(std::uint64_t value) -> const std::uint64_t {
            this->_fenceValue = value;
            return value;
        }

        [[nodiscard]] auto IncrementFenceValue() -> const std::uint64_t {
            return ++_fenceValue;
        }

        protected:		
        std::uint64_t _fenceValue = 0;

        protected:
            std::shared_ptr<Renderer::Allocator> _allocator;
            std::shared_ptr<Renderer::RenderTarget> _renderTarget;
    };
}
