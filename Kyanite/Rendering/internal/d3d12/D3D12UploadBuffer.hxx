#pragma once 

#include "UploadBuffer.hxx"
#include <d3d12.h>
#include <wrl.h>

namespace Renderer {
	class D3D12UploadBuffer: public UploadBuffer {
	public:
		D3D12UploadBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> buffer) {
			Buffer = buffer;
		}

		Microsoft::WRL::ComPtr<ID3D12Resource> Buffer;
	};
}