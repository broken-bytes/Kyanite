#pragma once

namespace Renderer {
    enum TextureFormat {
        RGBA = 0,
        RGBA_UINT = 1
    };

    enum class ResourceState {
        COMMON = 0x0,
        PRESENT = 0x0,
        VERTEX_CONST_BUFFER = 0x01,
        RENDER_TARGET = 0x4,
        PIXEL_SHADER = 0x80,
        COPY_DEST = 0x400,
        COPY_SOURCE = 0x800

    };

    enum class RendererLayer {
        DEFAULT = 0,
        USER_LAYER_1,
        USER_LAYER_2,
        USER_LAYER_3,
        USER_LAYER_4,
        USER_LAYER_5,
        USER_LAYER_6,
        USER_LAYER_7,
        USER_LAYER_8,
        USER_LAYER_9,
        USER_LAYER_10,
        USER_LAYER_11,
        BUILTIN_LAYER_12,
        BUILTIN_LAYER_13,
        BUILTIN_LAYER_14,
        BUILTIN_LAYER_15
    };
}
