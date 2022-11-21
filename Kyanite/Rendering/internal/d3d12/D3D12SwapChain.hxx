#pragma once

#include <SwapChain.hxx>
#include <d3d12/D3D12Device.hxx>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace Renderer {
	class D3D12SwapChain : public SwapChain {
	public:
		D3D12SwapChain(
			std::shared_ptr<CommandQueue> queue, 
			HWND window,
			D3D12Device& device,
			std::uint8_t frameCount,
			std::uint32_t width,
			std::uint32_t height
		);
		virtual auto Swap() -> void;
		[[nodiscard]] virtual auto CurrentBackBufferIndex() -> const std::uint64_t;
		[[nodiscard]] virtual auto GetBuffer(std::uint8_t frameIndex)->std::shared_ptr<RenderTarget>;


	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
	};
}