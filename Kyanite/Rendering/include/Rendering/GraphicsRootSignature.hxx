#pragma once

#include <cstdint>
#include <stddef.h>
#include <vector>

#undef DOMAIN

namespace Renderer {
    enum class GraphicsDescriptorRangeType: uint8_t {
        SRV	= 0,
        UAV	= 1,
        CBV	= 2,
        SAMPLER	= 3 
    };


    struct GraphicsDescriptorRange {
        GraphicsDescriptorRangeType Type;
        std::uint32_t Size;
        std::uint32_t ShaderRegister;
        std::uint32_t RegisterSpace;
    };

    enum class GraphicsShaderVisibility : uint8_t {
        ALL = 0,
        VERTEX = 1,
        HULL = 2,
        DOMAIN = 3,
        GEOMETRY = 4,
        PIXEL = 5,
        AMPLIFICATION = 6,
        MESH = 7
    };

    enum class GraphicsRootSignatureParameterType : uint8_t {
        CONSTANT = 0,
        TABLE = 1
    };

    struct GraphicsRootSignatureParameter {
        std::uint32_t Index;
        std::uint32_t Size;
        std::uint32_t ShaderRegister;
        std::uint32_t RegisterSpace;
        GraphicsShaderVisibility Visibility;
        GraphicsRootSignatureParameterType Type;
        GraphicsDescriptorRangeType RangeType;
    };

    struct GraphicsRootSignatureDescription {
        std::vector< GraphicsRootSignatureParameter> Parameters;
    };

    
    class GraphicsRootSignature {
        public:
        GraphicsRootSignature(GraphicsRootSignatureDescription& desc) {}
        friend class Device;
        virtual ~GraphicsRootSignature() {};
    };
}
