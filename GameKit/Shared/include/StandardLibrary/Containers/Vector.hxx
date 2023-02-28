#pragma once

#include "../../Common/PCH.hxx"
#include "../../Common/Macros.hxx"

namespace Containers {

    template<typename T>
    class Vector {
        
        public:
        Vector(uint64_t capacity) {
            this->_capacity = capacity;
            _data = (T*)malloc(_capacity);
        } 

        auto Push(T element) -> void {
            if(_size <= _capacity) {
                _capacity = _size * 10;
                auto newLocation = malloc(_capacity);
                memcmp(_data, newLocation, _size);
                _data = (T*)newLocation;
            }

            _data[_size++] = element;
        }

        auto Pop() -> T {
            return _data[_size--];
        }

        T& operator[](uint64_t index) {
            return _data[index];
        }

        [[nodiscard("Retrieving the capacity should not be done for no reason")]]
        auto Capacity() const -> uint64_t {
            return _capacity;
        }

        [[nodiscard("Retrieving the size should not be done for no reason")]]
        auto Size() const -> uint64_t {
            return _size;
        }

        private:
            T* _data;
            uint64_t _capacity;
            uint64_t _size;
    };

    template<typename T>
    SWIFT_EXPORTED(TinyVector.init(capacity:))
    inline auto Create(uint64_t capacity) -> Vector<T> {
        return Vector<T>(capacity);
    }
}