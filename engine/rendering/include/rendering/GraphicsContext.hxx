#pragma once

#include "CommandListType.hxx"
#include "Context.hxx"
#include "Device.hxx"
#include "IndexBuffer.hxx"
#include "VertexBuffer.hxx"
#include "PrimitiveTopology.hxx"

#include <memory>

namespace kyanite::engine::rendering {
    class RenderTarget;

    class GraphicsContext: public Context {
    public:
        GraphicsContext(const std::shared_ptr<Device>& device) : Context(CommandListType::Graphics, device) { }
        virtual ~GraphicsContext() = default;
        virtual auto ClearRenderTarget() -> void;
        virtual auto SetRenderTarget(std::shared_ptr<RenderTarget> target) -> void;
        virtual auto SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) -> void;
        virtual auto SetScissorRect(long left, long top, long right, long bottom) -> void;
        virtual auto SetPrimitiveTopology(PrimitiveTopology topology) -> void;
        virtual auto SetVertexBuffer(uint8_t index, std::shared_ptr<VertexBuffer>& buffer) -> void;
        virtual auto SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer) -> void;
        virtual auto DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) -> void;
    };
}