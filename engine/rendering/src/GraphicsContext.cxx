#include "rendering/GraphicsContext.hxx"

namespace kyanite::engine::rendering {
    auto GraphicsContext::ClearRenderTarget() -> void {
        _commandList->ClearRenderTarget({ 0.2f, 0.1f, 0.1f, 1.f });
    }

    auto GraphicsContext::SetRenderTarget(std::shared_ptr<RenderTarget> target) -> void {

    }

    auto GraphicsContext::SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) -> void {
        _commandList->SetViewport(x, y, width, height, minDepth, maxDepth);
    }

    auto GraphicsContext::SetScissorRect(long left, long top, long right, long bottom) -> void {
        _commandList->SetScissorRect(left, top, right, bottom);
    }

    auto GraphicsContext::SetPrimitiveTopology(PrimitiveTopology topology) -> void {
        _commandList->SetPrimitiveTopology(topology);
    }

    auto GraphicsContext::SetVertexBuffer(uint8_t index, std::shared_ptr<VertexBuffer>& buffer) -> void {

    }

    auto GraphicsContext::SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer) -> void {

    }

    auto GraphicsContext::DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) -> void {

    }
}