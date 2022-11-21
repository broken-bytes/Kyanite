#include <d3d12/D3D12Allocator.hxx>

namespace Renderer {
	D3D12Allocator::D3D12Allocator(CommandType type, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator): Allocator(type) {
		_allocator = allocator;
	}

	auto D3D12Allocator::Reset() -> void {
		_allocator->Reset();
	}

	auto D3D12Allocator::Native()->ID3D12CommandAllocator* {
		return _allocator.Get();
	}
}
