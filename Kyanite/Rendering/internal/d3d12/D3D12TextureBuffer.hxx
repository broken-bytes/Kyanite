#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#include <memory>
#include <cstdint>

#include "TextureBuffer.hxx"
using namespace Microsoft::WRL;

namespace Renderer {
	class D3D12CommandList;
	class D3D12TextureBuffer : public TextureBuffer {
	public:
		friend class D3D12Device;
		
		D3D12TextureBuffer(
			Microsoft::WRL::ComPtr<ID3D12Resource> buffer,
			std::string name
		);

		virtual auto Raw() ->ID3D12Resource* {
			return _buffer.Get();
		}

		~D3D12TextureBuffer() {};

	protected:
		ComPtr<ID3D12Resource> _buffer;
	};
}
