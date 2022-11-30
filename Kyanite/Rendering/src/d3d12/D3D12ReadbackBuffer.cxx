#include "d3d12/D3D12ReadbackBuffer.hxx"

namespace Renderer {
	D3D12ReadbackBuffer::D3D12ReadbackBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> buffer) {
		_buffer = buffer;
		buffer->SetName(L"ReadbackBuffer");

	}
}