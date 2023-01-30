#pragma once

#include <d3d12.h>
#include <memory>
#include <wrl.h>
#include "Allocator.hxx"

namespace Renderer {
	class D3D12Allocator : public Allocator {
	public:
		friend class Device;

		D3D12Allocator(CommandType type, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator, std::string name);

		virtual auto Reset() -> void;
		auto Native()->ID3D12CommandAllocator*;

	private:
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _allocator;
	};
}