#pragma once

#include <cstdint>

namespace rendering {
    class GpuResource {
    public:
        GpuResource(uint64_t address) : _address(address) { };
        virtual ~GpuResource() = default;
        [[nodiscard]] inline uint64_t Address() const { return _address; }

        static GpuResource* Create(uint64_t address) { return new GpuResource(address); }
    private:
        uint64_t _address;
    };
}