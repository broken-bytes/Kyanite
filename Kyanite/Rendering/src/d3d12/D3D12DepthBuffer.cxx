#include "d3d12/D3D12DepthBuffer.hxx"
#include <cassert>
#include <stdexcept>

namespace Renderer {
	auto D3D12DepthBuffer::Resize(uint32_t width, uint32_t height) -> void
	{
	}

	D3D12DepthBuffer::D3D12DepthBuffer(
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer,
		LPCWSTR name
	) {
		Buffer = buffer;
		// Update the depth-stencil view.
		D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
		dsv.Format = DXGI_FORMAT_D32_FLOAT;
		dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv.Texture2D.MipSlice = 0;
		dsv.Flags = D3D12_DSV_FLAG_NONE;
		View = dsv;
	}
}