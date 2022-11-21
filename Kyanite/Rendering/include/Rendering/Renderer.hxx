#pragma once

namespace Renderer {
    enum TextureFormat {
        RGBA = 0
    };

    enum class ResourceState {
        COMMON = 0,
        PRESENT = 0,
        VERTEX_CONST_BUFFER = 1,
        RENDER_TARGET = 0x4,
        PIXEL_SHADER = 0x80,
        COPY_DEST = 1024

    };
}
