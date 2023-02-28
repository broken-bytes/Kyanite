#pragma once

#include "Common/Macros.hxx"
#include "Common/PCH.hxx"

namespace WindowManager {

    SWIFT_EXPORTED(initialise())
    auto Initialise() -> int;

    struct WindowCreationDesc {
        const char* name;
        uint32_t width;
        uint32_t height;
        bool resizable;
        bool fullscreen;
    };

    SWIFT_EXPORTED(createWindow(description:))
    auto CreateWindow(WindowCreationDesc desc) -> Window;

    SWIFT_EXPORTED(handleWindowEvents())
    auto HandleWindowEvents() -> void;
}