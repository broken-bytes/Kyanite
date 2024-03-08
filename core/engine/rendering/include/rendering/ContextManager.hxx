#pragma once

#include "renderdoc_app.h"

#ifdef _WIN32
#define BEGIN_GRAPHICS_CTX(device) \
    rdoc_api->StartFrameCapture(nullptr, nullptr); \
    rdoc_api->SetCaptureOptionU32(nullptr, eRENDERDOC_Option_CaptureCallstacks, 1);     \
    rdoc_api->SetCaptureOptionU32(nullptr, eRENDERDOC_Option_RefAllResources, 1);       \
    rdoc_api->SetCaptureOptionU32(nullptr, eRENDERDOC_Option_SaveAllInitials, 1);       \
    rdoc_api->SetCaptureOptionU32(nullptr, eRENDERDOC_Option_CaptureAllCmdLists, 1);    \
    auto ctx = GraphicsContext::Create(device);

#define END_GRAPHICS_CTX(name) \
    rdoc_api->EndFrameCapture(nullptr, nullptr);
#else
#define BEGIN_GRAPHICS_CTX(name) \
    auto ctx = rdoc_api->StartFrameCapture(nullptr, nullptr);
    auto ctx = GraphicsContext::Create(device);

#define END_CTX(name) \
    rdoc_api->EndFrameCapture(nullptr, nullptr);

#endif