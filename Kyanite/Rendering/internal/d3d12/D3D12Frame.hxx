#pragma once 

#include <cstdint>
#include <d3d12.h>
#include "Allocator.hxx"
#include "Frame.hxx"
#include "RenderTarget.hxx"
#include <wrl.h>
#include <dxgi1_6.h>
#include "DescriptorHandle.hxx"

namespace Renderer {
	class D3D12Frame: public Frame {
	public:
		D3D12Frame(
			std::shared_ptr<Renderer::Allocator> allocator,
			std::shared_ptr<Renderer::RenderTarget> renderTarget
		);
	};
}
