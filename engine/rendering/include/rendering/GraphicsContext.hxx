#pragma once

#include "CommandListType.hxx"
#include "Context.hxx"
#include "Device.hxx"
#include <memory>

namespace kyanite::engine::rendering {
    class RenderTarget;

    class GraphicsContext: public Context {
    public:
        GraphicsContext(const std::shared_ptr<Device> device) : Context(CommandListType::Graphics, device) { }
        virtual ~GraphicsContext() = default;
        virtual auto ClearRenderTarget() -> void;
        virtual auto SetRenderTarget(std::shared_ptr<RenderTarget> target) -> void;

        static auto Create(const std::shared_ptr<Device> device) -> std::unique_ptr<GraphicsContext> {
            return std::make_unique<GraphicsContext>(device);
        }
    };
}