#pragma once 

#include <cstdint>
#include "DescriptorHandle.hxx"

namespace Renderer {
    class Heap {
        public:
        Heap() {
        }

        virtual ~Heap() {

        }

        virtual auto CpuHandleFor(std::uint32_t index) -> std::shared_ptr<DescriptorHandle> = 0;
        virtual auto GpuHandleFor(std::uint32_t index) -> std::shared_ptr<DescriptorHandle> = 0;
    };
}
