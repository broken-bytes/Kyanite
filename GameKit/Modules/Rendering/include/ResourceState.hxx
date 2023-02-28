#pragma once

namespace Rendering {

    enum class ResourceState {

        COMMON = 0x0,
        PRESENT = 0x0,
        VERTEX_CONST_BUFFER = 0x01,
        RENDER_TARGET = 0x4,
        PIXEL_SHADER = 0x80,
        COPY_DEST = 0x400,
        COPY_SOURCE = 0x800
    };
}