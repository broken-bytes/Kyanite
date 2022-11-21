#pragma once

#include <vector>
#include <stddef.h>

#include "Buffer.hxx"

namespace Renderer {
class IndexBuffer: public Buffer {
    public:
    friend class Device;
    
    virtual auto ClearStaging() -> void = 0;

    virtual auto Data() -> void* {
        return _indices.data();
    }

    protected:
    std::vector<std::uint32_t> _indices = {};
    std::uint32_t _index;
    
    IndexBuffer(std::vector<std::uint32_t> indices) {
        this->_indices = indices;
    }


};
}