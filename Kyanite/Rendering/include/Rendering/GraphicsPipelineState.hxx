#pragma once

#include <memory>
#include <vector>

#include "GraphicsRootSignature.hxx"

namespace Renderer {
    class GraphicsShaderBinding;

    enum class GraphicsPipelineFormat {
        UNDEFINED           = 0,
        RGBA32_UNDEFINED    = 1,
        RGBA32_FLOAT        = 2,
        RGBA32_UINT         = 3,
        RGBA32_SINT         = 4,
        RGB32_UNDEFINED     = 5,
        RGB32_FLOAT         = 6,
        RGB32_UINT          = 7,
        RGB32_SINT          = 8,
        RG32_UNDEFINED      = 15,
        RG32_FLOAT          = 16,
        RG32_UINT           = 17,
        RG32_SINT           = 18,
    };

    enum class GraphicsPipelineClassification {
        VERTEX = 0,
        INSTANCE = 1
    };

    enum class GraphicsPipelineStateTopology: uint8_t {
        POLYGON = 4,
        WIREFRAME = 2
    };

    struct GraphicsPipelineInputElement {
        const char* SemanticName;
        std::uint32_t SemanticIndex;
        GraphicsPipelineFormat Format;
        std::uint32_t InputSlot;
        std::uint32_t AlignedByteOffset;
        GraphicsPipelineClassification Classification;
        std::uint32_t InstanceDataStepRate;
    };

    constexpr std::uint8_t PIPELINE_STATE_ENABLE_DEPTH = 1;
    constexpr std::uint8_t PIPELINE_STATE_ENABLE_STENCIL = 1;

    class GraphicsPipelineState {
        public:
        friend class Device;

        GraphicsPipelineState(
            std::shared_ptr<Renderer::GraphicsRootSignature> rootSignature,
            std::vector<GraphicsPipelineInputElement> inputLayout,
            std::shared_ptr<GraphicsShaderBinding> shaderBinding,
            std::uint8_t flags,
            GraphicsPipelineStateTopology topology
        ) {
            _rootSignature = rootSignature;
            _inputLayout = inputLayout;
        }

        [[nodiscard]] virtual auto RootSignature()->std::shared_ptr<Renderer::GraphicsRootSignature> {
            return _rootSignature;
        }

        protected:
        std::shared_ptr<Renderer::GraphicsRootSignature> _rootSignature;
        std::vector<GraphicsPipelineInputElement> _inputLayout;     
    };
}