#include "d3d12/D3D12Frame.hxx"
#include "RenderTarget.hxx"

namespace Renderer {
	D3D12Frame::D3D12Frame(
		std::shared_ptr<Renderer::Allocator> allocator,
		std::shared_ptr<Renderer::RenderTarget> renderTarget
	) : Frame(allocator, renderTarget) {

	}

	auto SetRendertarget(std::shared_ptr<Renderer::RenderTarget> renderTarget) -> void {
		
	}
}