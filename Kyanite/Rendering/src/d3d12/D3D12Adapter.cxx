#include <d3d12/D3D12Adapter.hxx>

namespace Renderer {
	auto D3D12Adapter::Adapters() -> AdapterList
	{
		AdapterList list = {};
		UINT dxgiFactoryFlags = 0;
		
		ComPtr<IDXGIAdapter1> adapter;
		ComPtr<IDXGIAdapter4> adapter4;
		ComPtr<IDXGIFactory2> factory2;
		ComPtr<IDXGIFactory7> factory7;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory2));
		if (SUCCEEDED(factory2->QueryInterface(IID_PPV_ARGS(&factory7)))) {
			for (
				UINT adapterIndex = 0;
				SUCCEEDED(factory7->EnumAdapterByGpuPreference(
					adapterIndex,
					DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
					IID_PPV_ARGS(&adapter)));
				++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);


				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					continue;
				}

				// Check to see whether the adapter supports Direct3D 12, but don't create the
				// actual device yet.
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, _uuidof(ID3D12Device), nullptr)))
				{
					adapter.As(&adapter4);
					list.emplace_back(new D3D12Adapter(adapter4.Detach()));
				}
			}
		}

		return list;
	}

	auto D3D12Adapter::Description() -> DXGI_ADAPTER_DESC3
	{
		return _desc;
	}

	auto D3D12Adapter::Name() -> const wchar_t*
	{
		return _desc.Description;
	}

	auto D3D12Adapter::VRAM() -> uint64_t
	{
		return _desc.DedicatedVideoMemory;
	}

	auto D3D12Adapter::Raw() -> IDXGIAdapter4*
	{
		return _adapter.Get();
	}

	D3D12Adapter::D3D12Adapter(IDXGIAdapter4* adapter)
	{
		_adapter = adapter;
		_adapter->GetDesc3(&_desc);
	}
}