#include "d3d12/D3D12TextureBuffer.hxx"

namespace Renderer {
	D3D12TextureBuffer::D3D12TextureBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> buffer) {
		_buffer = buffer;
		buffer->SetName(L"Texture");

	}
}