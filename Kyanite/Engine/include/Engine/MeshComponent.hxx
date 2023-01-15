#pragma once

#include <memory>

struct NativeRef;

struct MeshComponent {
    std::unique_ptr<NativeRef> ModelGPURef;
};