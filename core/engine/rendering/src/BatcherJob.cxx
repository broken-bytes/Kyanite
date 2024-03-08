#include "rendering/BatcherJob.hxx"

#include <algorithm>

namespace kyanite::engine::rendering {
    auto BatcherJob::Execute(std::shared_ptr<CommandList>& commandList) -> void {

    }

    auto BatcherJob::SubmitDrawCall(
        uint64_t sortKey, 
        std::shared_ptr<VertexBuffer>& vertexBuffer,
        std::shared_ptr<IndexBuffer>& indexBuffer,
        std::shared_ptr<Material>& material
    ) -> void {
        _drawCalls.emplace_back(sortKey, vertexBuffer, indexBuffer, material);
        // Sort the draw calls by sort key
        std::sort(_drawCalls.begin(), _drawCalls.end(), [](const DrawCall& a, const DrawCall& b) {
			return a.sortKey < b.sortKey;
		});
    }
}