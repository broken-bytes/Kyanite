#include <cstdint>
#include <d3d12.h>
#include "d3d12/D3D12Fence.hxx"

namespace Renderer {
	D3D12Fence::D3D12Fence(D3D12Device& device, std::uint64_t value): Fence(value) {
		auto dev = device;
		device.Native()->CreateFence(value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	}

	auto D3D12Fence::GetCompletedValue()->std::uint64_t {
		return _fence->GetCompletedValue();
	}

	auto D3D12Fence::SetOnCompletion(std::uint64_t value, HANDLE event) -> void {
		_fence->SetEventOnCompletion(value, event);
	}

	auto D3D12Fence::Wait(std::uint64_t value, HANDLE event) -> void {
		if (_fence->GetCompletedValue() < value) {
			_fence->SetEventOnCompletion(value, event);
			WaitForSingleObjectEx(event, INFINITE, FALSE);
		}
	}

	auto D3D12Fence::Native()->ID3D12Fence* {
		return _fence.Get();
	}
}