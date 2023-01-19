#pragma once 

#include <memory.h>

#include "Buffer.hxx"
#include "DescriptorHandle.hxx"

namespace Renderer {
    class TextureBuffer: public Buffer {
        public:
            std::shared_ptr<GPUDescriptorHandle> GPUHandle;
        TextureBuffer() {}
        virtual ~TextureBuffer() {

        };
        private:
    };
}
