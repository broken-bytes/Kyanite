#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#include <cstdint>
#include <wrl.h>
#include "Buffer.hxx"

#include "d3d12/D3D12Buffer.hxx"
#include "d3d12/D3D12Resource.hxx"

namespace Renderer {
	class D3D12DepthBuffer: public Buffer, public D3D12Resource {
	public:

		D3D12DepthBuffer(
			Microsoft::WRL::ComPtr<ID3D12Resource> buffer,
			LPCWSTR name = L"DepthBuffer"
		);
		auto Resize(std::uint32_t width, std::uint32_t height) -> void;

		auto Resource() ->ID3D12Resource* {
			return Buffer.Get();
		}

		D3D12_DEPTH_STENCIL_VIEW_DESC View;
		Microsoft::WRL::ComPtr<ID3D12Resource> Buffer;
	};
}
