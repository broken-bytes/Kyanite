#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "RenderTarget.hxx"
#include "d3d12/D3D12Resource.hxx"

namespace Renderer {
	class D3D12RenderTarget : public RenderTarget {
	public:
		D3D12RenderTarget(Microsoft::WRL::ComPtr<ID3D12Resource> renderTarget) {
			_renderTarget = renderTarget;
		}

		auto Resource() ->ID3D12Resource* {
			return _renderTarget.Get();
		}

		auto Depth() ->ID3D12Resource* {
			return _depthTarget.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _renderTarget;
		Microsoft::WRL::ComPtr<ID3D12Resource> _depthTarget;
	};
}