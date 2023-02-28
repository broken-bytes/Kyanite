#pragma once

#include "RenderLayer.hxx"

#include "Math/Vector3.hxx"
#include "Math/Vector4.hxx"

namespace Rendering {

    enum class RenderBackendType {

        D3D12,
        OpenGL,
        Vulkan,
        Metal,
    };

    class RenderBackend {

        public:
        RenderBackend(void* window, uint16_t resolutionX, uint16_t resolutionY);
        virtual ~RenderBackend() {}
        virtual auto Draw(Rendering::RenderLayer layer, uint8_t viewPort, uint64_t materialId, uint64_t meshId, Math::Vector3 position, Math::Vector4 rotation, Math::Vector3 scale) -> void = 0;
    };
}