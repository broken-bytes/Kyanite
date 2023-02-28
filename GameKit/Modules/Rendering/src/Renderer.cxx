#include "Renderer.hxx"

#include "Device.hxx"
#include "Frame.hxx"
#include "RenderLayer.hxx"
#include "DrawCall.hxx"
#include "d3d12/D3D12Device.hxx"

#include <SDL.h>

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Rendering {
    constexpr uint8_t NUM_FRAMES = 2;

    namespace Renderer {

        std::unique_ptr<Device> GPU;
        std::array<std::shared_ptr<Frame>, NUM_FRAMES> Frames;
        std::vector<Rendering::GPUTask> GpuTasks = {};

        void Init(Window window, RenderBackendType type, uint32_t width, uint32_t height) {
            switch (type) {
                case RenderBackendType::D3D12:
                #ifndef _WIN32
                throw std::runtime_error("Direct3D12 unailable");
                #endif
                GPU = D3D12Device::MakeD3D12Device(window);
                break;
                case RenderBackendType::OpenGL:
                case RenderBackendType::Vulkan:
                case RenderBackendType::Metal:
                #ifndef __APPLE__
                throw std::runtime_error("Metal unailable");
                #endif
                break;
            }

            std::cout << "Renderer Init Done" << std::endl;
        }

        void Draw(
            Rendering::RenderLayer layer, 
            uint8_t viewPort, 
            uint64_t materialId, 
            uint64_t meshId, 
            Math::Vector3 position, 
            Math::Vector4 rotation, 
            Math::Vector3 scale
        ) {   
        
        }
    }
}