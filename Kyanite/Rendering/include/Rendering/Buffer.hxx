#pragma once 

#include <cstdint>
#include <stddef.h>
#include "Resource.hxx"


namespace Renderer {
    class Buffer: public Resource {
        public:
        Buffer() {

        }
        virtual ~Buffer() {
            
        }

        virtual auto Size()->size_t {
            return Count;
        }

        size_t Count;
        private:
    };
}
