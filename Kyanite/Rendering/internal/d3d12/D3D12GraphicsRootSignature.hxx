#pragma once

#include "GraphicsRootSignature.hxx"
#include "D3D12Device.hxx"
#include <d3dx12.h>
#include <wrl.h>

namespace Renderer {
	class D3D12GraphicsRootSignature : public GraphicsRootSignature {
	public:
		friend class D3D12Device;

		D3D12GraphicsRootSignature(D3D12Device& device, GraphicsRootSignatureDescription& desc);
		~D3D12GraphicsRootSignature();

		[[nodiscard]] auto Native()->ID3D12RootSignature*;

	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;
	};
}