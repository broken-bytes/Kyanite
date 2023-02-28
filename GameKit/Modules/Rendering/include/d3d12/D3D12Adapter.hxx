#pragma once

#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include <memory>
#include <string>
#include <vector>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Rendering {

	class D3D12Adapter;

	typedef std::vector<std::shared_ptr<D3D12Adapter>> AdapterList;

	class D3D12Adapter {
	public:
		[[nodiscard("Dont query adapters list without using it")]] 
        static auto Adapters()->AdapterList;
		[[nodiscard("Dont query adapter description without using it")]] 
        auto Description()->DXGI_ADAPTER_DESC3;
        [[nodiscard("Dont query adapter name without using it")]] 
		auto Name()->const wchar_t*;
        [[nodiscard("Dont query adapter vram size without using it")]] 
        auto VRAM()->uint64_t;
		[[nodiscard("Dont get a raw adapter without using it")]] 
        virtual auto Raw()->IDXGIAdapter4*;
	protected:
		D3D12Adapter(IDXGIAdapter4* adapter);
	private:
		DXGI_ADAPTER_DESC3 _desc;
		Microsoft::WRL::ComPtr<IDXGIAdapter4> _adapter;
	};
}