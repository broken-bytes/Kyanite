#pragma once

#include "Renderer.hxx"
#include <map>
#include <string>
#include <vector>

namespace Renderer {
    enum class GraphicsShaderSlotType {
        TEXTURE = 0,
        BOOL    = 1,
        INT     = 2,
        FLOAT   = 3,
        VECTOR2 = 4,
        VECTOR3 = 5,
        VECTOR4 = 6
    };

    struct GraphicsShaderSlot {
        std::string Name;
        GraphicsShaderSlotType Type;
        uint8_t Index;
    };

    struct GraphicsShader {
        std::string Name;
        TextureFormat Format;
        std::vector<GraphicsShaderSlot> Constants;
        std::vector<GraphicsShaderSlot> ConstantBufferLayout;
        bool IsLit;
        // Name, Code
        std::string Code;
    };
}