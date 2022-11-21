#pragma once

#include <cstdint>
#include <vector>
#include <SDL.h>


((*Func1)auto AddRef(NativeRef* objc) -> void));


namespace Renderer {
    auto Init(uint32_t width, uint32_t height) -> void;
    auto Resize(uint32_t width, uint32_t height) -> void;
    auto UploadMeshData(Mesh* mesh) -> void;
}