#pragma once 

#include <cstdint>
#include <wrl.h>
#include <d3d12.h>
#include "D3D12Device.hxx"
#include "Fence.hxx"


namespace Renderer {
	class D3D12Fence: public Fence {
	public:
		D3D12Fence(D3D12Device& device, std::uint64_t value);
		[[nodiscard]] auto GetCompletedValue()->std::uint64_t;
		auto SetOnCompletion(uint64_t value, HANDLE event) -> void;
		auto Wait(std::uint64_t value, HANDLE event) -> void;

		auto Native()->ID3D12Fence*;

	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> _fence = { nullptr };
	};
}
