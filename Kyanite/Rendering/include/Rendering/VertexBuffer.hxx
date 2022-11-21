#pragma once

#include <cstdint>
#include <stddef.h>
#include <vector>
#include "Buffer.hxx"
#include "Vertex.hxx"

namespace Renderer {
class VertexBuffer: public Buffer {
    public:
    friend class Device;
    
    virtual auto ClearStaging() -> void = 0;

    protected:
    std::vector<Vertex> _vertices = {};
    std::uint32_t _index;
    
    VertexBuffer(std::vector<Vertex> vertices) {
        this->_vertices = vertices;
    }

};
}