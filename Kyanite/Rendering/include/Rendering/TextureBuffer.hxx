#pragma once 

#include "Buffer.hxx"
#include "DescriptorHandle.hxx"

namespace Renderer {
    class TextureBuffer: public Buffer {
        public:
            std::shared_ptr<DescriptorHandle> GPUHandle;
        TextureBuffer() {}
        virtual ~TextureBuffer() {

        };
        private:
    };
}
