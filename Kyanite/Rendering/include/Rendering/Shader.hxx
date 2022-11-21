#pragma once

#include "GraphicsPipelineState.hxx"

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
        GraphicsShaderSlotType Type;
        const char* SlotName;
    };

    struct GraphicsShader {
        const char* Name;
        std::vector<GraphicsShaderSlot> Slots;
        const char* Code;
    };
}