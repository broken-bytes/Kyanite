#pragma once

#include <memory>
#include "GraphicsPipelineState.hxx"
#include "Shader.hxx"

namespace Renderer {
    class GraphicsShaderBinding {
    public:
        std::uint64_t ShaderIndex;
        std::shared_ptr<GraphicsPipelineState> Pipeline;
    };
}