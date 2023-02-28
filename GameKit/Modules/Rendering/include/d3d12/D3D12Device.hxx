#pragma once

#include "Device.hxx"

#include "Common/PCH.hxx"

#include <SDL.h>
#include <directx/d3d12.h>
#include <memory>
#include <wrl.h>

namespace Rendering {

    class D3D12Device: public Device {
        
        public:
        D3D12Device(Window window);
        ~D3D12Device();
        virtual auto CreateSwapchain(uint8_t numBuffers, uint32_t width, uint32_t height) -> Swapchain;
        virtual auto CreateVertexBuffer() -> Buffer;
        virtual auto CreateIndexBuffer() -> Buffer;
        virtual auto GetList(CommandList::CommandListType type, uint8_t index) -> CommandList;
        virtual auto GetQueue(CommandList::CommandListType type) -> CommandQueue;

        static inline auto MakeD3D12Device(Window window) -> std::unique_ptr<Device> {
            return std::make_unique<D3D12Device>(window);
        }

        private:
        Microsoft::WRL::ComPtr<ID3D12Device10> _device;
    };
}