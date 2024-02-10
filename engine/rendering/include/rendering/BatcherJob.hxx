#pragma once

#include "GraphicsContext.hxx"

#include <cstdint>
#include <functional>
#include <memory>

namespace kyanite::engine::rendering {
    class BatcherJob {
    public:
        /**
        * @brief Executes the job and sends the draw calls to the GPU
        *
        */
        auto Execute(std::shared_ptr<GraphicsContext>& context) -> void;

        /**
        * @brief Adds a draw call to the job
        *
        * @param sortKey The sort key of the draw call
        * @param drawCall the draw call to add
        */
        auto SubmitDrawCall(uint64_t sortKey, std::function<void()> drawcall) -> void;
    };
}