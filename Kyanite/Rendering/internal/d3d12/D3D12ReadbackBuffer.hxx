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
			Microsoft::WRL::ComPtr<ID3D12Resource> buffer,
			size_t sizeInBytes
		);
		virtual auto Raw() ->ID3D12Resource* {
			return _buffer.Get();
		}

		~D3D12ReadbackBuffer() {};

		virtual auto ReadBytes() -> uint32_t* override;

	protected:
		ComPtr<ID3D12Resource> _buffer;
		uint32_t* _bufferData;
		uint8_t* _tempBuffer;
		size_t _sizeInBytes;

	};
}
