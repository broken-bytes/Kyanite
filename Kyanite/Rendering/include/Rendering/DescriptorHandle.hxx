#pragma once


namespace Renderer {

    class DescriptorHandle {
    public:
        DescriptorHandle(uint64_t address) {
            this->_address = address;
        }

        virtual auto Address() -> uint64_t {
            return _address;
        }

        virtual ~DescriptorHandle() = 0 {};

    protected:
        uint64_t _address;
    };

    class GPUDescriptorHandle: public DescriptorHandle {
    public:
        GPUDescriptorHandle(uint64_t address) : DescriptorHandle(address) {}
    };

    class CPUDescriptorHandle : public DescriptorHandle {
    public:
        CPUDescriptorHandle(uint64_t address) : DescriptorHandle(address) {}
    };
}