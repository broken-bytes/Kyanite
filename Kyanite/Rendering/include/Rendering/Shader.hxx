#pragma once

#include <map>
#include <string>
#include <vector>

namespace Renderer {

    enum class GraphicsShaderStageType: uint8_t {
        VERTEX,
        PIXEL,
        MESH
    };
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
        std::string Name;
        std::vector<GraphicsShaderSlot> Slots;
        // Name, Code
        std::string Code;
    };
}