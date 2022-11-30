#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#include <memory>
#include <cstdint>

#include "ReadbackBuffer.hxx"
using namespace Microsoft::WRL;

namespace Renderer {
	class D3D12CommandList;
	class D3D12ReadbackBuffer : public ReadbackBuffer {
	public:
		friend class D3D12Device;
		
		D3D12ReadbackBuffer(
			Microsoft::WRL::ComPtr<ID3D12Resource> buffer
		);
		virtual auto Raw() ->ID3D12Resource* {
			return _buffer.Get();
		}

		~D3D12ReadbackBuffer() {};

	protected:
		ComPtr<ID3D12Resource> _buffer;
	};
}
