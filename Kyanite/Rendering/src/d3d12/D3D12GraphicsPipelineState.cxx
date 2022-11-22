#include <d3dx12.h>

#include "d3d12/D3D12GraphicsPipelineState.hxx"
#include "d3d12/D3D12GraphicsRootSignature.hxx"
#include "d3d12/D3D12ShaderBinding.hxx"


namespace Renderer {
    D3D12GraphicsPipelineState::D3D12GraphicsPipelineState(
		D3D12Device& device,
        std::shared_ptr<Renderer::GraphicsRootSignature> rootSignature,
        std::vector<GraphicsPipelineInputElement> inputLayout,
        std::shared_ptr<GraphicsShaderBinding> shaderBinding,
        std::uint8_t flags,
        GraphicsPipelineStateTopology topology
    ): GraphicsPipelineState(rootSignature, inputLayout, shaderBinding, flags, topology) {
		struct PipelineStateConfig
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER Rasterizer;
		};

		_rootSignature = rootSignature;


		std::vector<D3D12_INPUT_ELEMENT_DESC> layout = {};

		for (const auto& item : inputLayout) {
			D3D12_INPUT_ELEMENT_DESC desc = {item.SemanticName, item.SemanticIndex, DXGI_FORMAT(item.Format), item.InputSlot, item.AlignedByteOffset, D3D12_INPUT_CLASSIFICATION(item.Classification), item.InstanceDataStepRate};
			layout.push_back(desc);
		}

		auto d3d312Shader = *static_pointer_cast<D3D12GraphicsShaderBinding>(shaderBinding);

		CD3DX12_SHADER_BYTECODE vertexShader = { d3d312Shader.VertexShader.Get() };
		CD3DX12_SHADER_BYTECODE pixelShader = { d3d312Shader.PixelShader.Get()};

		PipelineStateConfig config{};
		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		CD3DX12_RASTERIZER_DESC rasterDesc = {};
		rasterDesc.MultisampleEnable = false;
		rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
		config.Rasterizer = rasterDesc;
		config.pRootSignature = static_pointer_cast<D3D12GraphicsRootSignature>(rootSignature)->Native();
		config.InputLayout = { layout.data(), (unsigned int)layout.size() };
		config.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		config.VS = vertexShader;
		config.PS = pixelShader;
		config.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		config.RTVFormats = rtvFormats;

		D3D12_PIPELINE_STATE_STREAM_DESC desc = {
			sizeof(PipelineStateConfig), &config
		};

		device.Native()->CreatePipelineState(&desc, IID_PPV_ARGS(&_pipelineState));
		auto t = 0;
    }

	auto D3D12GraphicsPipelineState::Native() const->ID3D12PipelineState* {
		return _pipelineState.Get();
	}
}