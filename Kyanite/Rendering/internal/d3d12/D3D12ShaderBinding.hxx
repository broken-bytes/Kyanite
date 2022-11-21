#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <ShaderBinding.hxx>

namespace Renderer {
	class D3D12GraphicsShaderBinding : public GraphicsShaderBinding {
	public:
		Microsoft::WRL::ComPtr<ID3DBlob> VertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> PixelShader;
	};
}