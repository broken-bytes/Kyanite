#include <d3d12/D3D12GraphicsRootSignature.hxx>

#include <algorithm>
#include <stdexcept>

namespace Renderer {
	D3D12GraphicsRootSignature::D3D12GraphicsRootSignature(
		D3D12Device& device,
		GraphicsRootSignatureDescription& desc
	) : GraphicsRootSignature(desc)
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (device.Native()->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)) != S_OK)
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;

		std::vector< D3D12_STATIC_SAMPLER_DESC> samplers;

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplers.push_back(sampler);


		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


		std::vector<CD3DX12_ROOT_PARAMETER1> rootParameters(desc.Parameters.size());

		std::vector<std::vector<CD3DX12_DESCRIPTOR_RANGE1>> ranges = {};


		for (const auto& param : desc.Parameters) {
			if (param.Type == GraphicsRootSignatureParameterType::CONSTANT) {
				rootParameters[param.Index].InitAsConstants(param.Size, param.ShaderRegister, 0);
			}
			else {
				std::vector< CD3DX12_DESCRIPTOR_RANGE1> currRange;
				CD3DX12_DESCRIPTOR_RANGE1 range = {};
				range.Init(D3D12_DESCRIPTOR_RANGE_TYPE(param.RangeType), 1, param.ShaderRegister, param.RegisterSpace, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);
				currRange.push_back(range);
				ranges.push_back(currRange);
				rootParameters[param.Index].InitAsDescriptorTable(1, ranges[ranges.size() -1].data());
			}
		}

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(rootParameters.size(), rootParameters.data(), 1, &sampler, rootSignatureFlags);

		auto result = D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &signature, &error);
		if (S_OK != result) {
			auto err = (char*)error->GetBufferPointer();
			OutputDebugString(err);
			throw std::runtime_error((char*)error.Get());
		}
		else {
			device.Native()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
		}
	}

	D3D12GraphicsRootSignature::~D3D12GraphicsRootSignature()
	{

	}

	auto D3D12GraphicsRootSignature::Native()->ID3D12RootSignature* {
		return _rootSignature.Get();
	}
}