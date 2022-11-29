#pragma once

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "GraphicsPipelineState.hxx"
#include "Shader.hxx"

namespace Renderer {
    enum class ShaderPropType {
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        INT,
        BOOL
    };

    class GraphicsShaderBinding {
    public:
        std::string Name;
        std::uint64_t ShaderIndex;
        bool IsLit;
        std::vector<GraphicsShaderSlot> Slots;
        std::shared_ptr<GraphicsPipelineState> Pipeline;
    };
}