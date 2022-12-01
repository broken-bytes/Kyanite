#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Buffer.hxx"
#include "DescriptorHandle.hxx"

namespace Renderer {
    class ReadbackBuffer: public Buffer {
        public:
            std::shared_ptr<DescriptorHandle> GPUHandle;
            ReadbackBuffer() {}
            virtual ~ReadbackBuffer() {};
            virtual auto ReadBytes() -> uint32_t* = 0;
        private:
    };
}
