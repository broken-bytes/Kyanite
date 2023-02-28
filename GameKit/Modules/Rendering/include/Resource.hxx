#pragma once

#include "Common/PCH.hxx"

namespace Rendering {
    
    struct Resource {

        uint64_t GPUAddress;
        uint64_t CPUAddress;
    };
}