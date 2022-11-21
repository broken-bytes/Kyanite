#include <d3d12/D3D12SwapChain.hxx>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl.h>
#include <d3d12/D3D12CommandQueue.hxx>
#include <d3d12/D3D12RenderTarget.hxx>

namespace Renderer {
	D3D12SwapChain::D3D12SwapChain(
		std::shared_ptr<CommandQueue> queue,
		HWND window,
		D3D12Device& device,
		std::uint8_t frameCount,
		std::uint32_t width,
		std::uint32_t height) : SwapChain(queue, window, frameCount, width, height) {
		UINT dxgiFactoryFlags = 0;
#if _DEBUG

		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}

#endif
		Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = width;
		swapChainDesc.Height = height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = frameCount;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;

		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		auto result = factory->CreateSwapChainForHwnd(
			static_pointer_cast<D3D12CommandQueue>(queue)->Native(),
			window,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		);

		swapChain.As(&_swapChain);

	}

	auto D3D12SwapChain::Swap() -> void {
		_swapChain->Present(0, 0);
	}

	auto D3D12SwapChain::CurrentBackBufferIndex() -> const std::uint64_t {
		return _swapChain->GetCurrentBackBufferIndex();
	}

	auto D3D12SwapChain::GetBuffer(std::uint8_t frameIndex)->std::shared_ptr<RenderTarget> {
		Microsoft::WRL::ComPtr<ID3D12Resource> rt;
		_swapChain->GetBuffer(frameIndex, IID_PPV_ARGS(&rt));
		return std::make_shared<D3D12RenderTarget>(rt);
	}

}