#pragma once


namespace Renderer {

class DescriptorHandle {
    
};

template<typename T>
class DescriptorHandleT: public DescriptorHandle {
    public:
    DescriptorHandleT(T type) {
        _handle = type;
    }

    auto Handle() -> T {
        return _handle;
    }

    private:
    T _handle;
    
};
}