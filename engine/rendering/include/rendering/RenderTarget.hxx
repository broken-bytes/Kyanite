#pragma once

#include "GpuResource.hxx"

namespace rendering {
	class RenderTarget : public GpuResource {
	public:
        explicit RenderTarget(uint64_t address) : GpuResource(address) {}

        virtual ~RenderTarget() = default;
	};
}