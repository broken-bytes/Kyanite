#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <memory>


using namespace Microsoft::WRL;

namespace Renderer {
	class D3D12Adapter;
	typedef std::vector<std::shared_ptr<D3D12Adapter>> AdapterList;

	class D3D12Adapter {
	public:
		[[nodiscard]] static auto Adapters()->AdapterList;
		[[nodiscard]] auto Description()->DXGI_ADAPTER_DESC3;
		[[nodiscard]] auto Name()->const wchar_t*;
		[[nodiscard]] auto VRAM()->uint64_t;
		[[nodiscard]] virtual auto Raw()->IDXGIAdapter4*;
	protected:
		D3D12Adapter(IDXGIAdapter4* adapter);
	private:
		DXGI_ADAPTER_DESC3 _desc;
		Microsoft::WRL::ComPtr<IDXGIAdapter4> _adapter;
	};
}