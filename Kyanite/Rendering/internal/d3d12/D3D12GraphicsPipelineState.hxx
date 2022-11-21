#pragma once

#include <d3d12/D3D12Device.hxx>
#include <GraphicsPipelineState.hxx>
#include <ShaderBinding.hxx>
#include <wrl.h>

namespace Renderer {
	class D3D12GraphicsPipelineState : public GraphicsPipelineState {
    public:
        friend class D3D12Device;

        D3D12GraphicsPipelineState(
            D3D12Device& device,
            std::shared_ptr<Renderer::GraphicsRootSignature> rootSignature,
            std::vector<GraphicsPipelineInputElement> inputLayout,
            std::shared_ptr<GraphicsShaderBinding> shaderBinding,
            std::uint8_t flags,
            GraphicsPipelineStateTopology topology
        );

        auto Native() const->ID3D12PipelineState*;

    private:
        Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;
	};
}