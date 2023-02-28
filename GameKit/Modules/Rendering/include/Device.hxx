#pragma once

#include "Buffer.hxx"
#include "CommandList.hxx"
#include "CommandQueue.hxx"
#include "Swapchain.hxx"

#include "Common/PCH.hxx"

#include <string>

namespace Rendering {

    class Device {

        public:
        Device(Window window) : _window(window) {}
        virtual ~Device() {}
        virtual auto CreateSwapchain(uint8_t numBuffers, uint32_t width, uint32_t height) -> Swapchain = 0;
        virtual auto CreateVertexBuffer() -> Buffer = 0;
        virtual auto CreateIndexBuffer() -> Buffer = 0;
        virtual auto GetList(CommandList::CommandListType type, uint8_t index) -> CommandList = 0;
        virtual auto GetQueue(CommandList::CommandListType type) -> CommandQueue = 0;

        protected:
        Window _window;
    };
}