#pragma once

#include <cassert>
#include <cstdint>
#include <d3d12.h>
#include <d3dx12.h>
#include <stdexcept>
#include <sstream>
#include <wrl.h>
#include "DescriptorHandle.hxx"
#include "D3D12Device.hxx"


namespace Renderer {
	class D3D12Heap : public Heap {
	public:
		D3D12Heap(
			D3D12Device& device,
			D3D12_DESCRIPTOR_HEAP_TYPE type,
			std::uint32_t numDescriptors,
			bool shaderVisible = false
		) {
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.NumDescriptors = numDescriptors;
			desc.Type = type;
			desc.Flags = (shaderVisible) ?
				D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE :
				D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			if (S_OK == device.Native()->CreateDescriptorHeap(
				&desc,
				IID_PPV_ARGS(&_heap))
				) {
				_desc = desc;
				_increment = device.Native()->GetDescriptorHandleIncrementSize(type);
				_type = type;
				_isShaderVisible = shaderVisible;
				_heapStartCPU = _heap->GetCPUDescriptorHandleForHeapStart();
				if (shaderVisible) {
					_heapStartGPU = _heap->GetGPUDescriptorHandleForHeapStart();
				}
			}
			else {
				throw std::runtime_error("Failed to create Heap");
			}
		};

		[[nodiscard]] 
		virtual auto Size()->size_t {
			return _size;
		};

		[[nodiscard]]
		virtual auto Increment()->size_t {
			return _increment;
		};

		[[nodiscard]] 
		virtual auto CPUHandleForHeapStart() -> DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE> {
			return { _heap->GetCPUDescriptorHandleForHeapStart()};
		};

		[[nodiscard]]
		virtual auto GPUHandleForHeapStart() -> DescriptorHandleT<D3D12_GPU_DESCRIPTOR_HANDLE>{
			return { _heap->GetGPUDescriptorHandleForHeapStart()};
		};

		[[nodiscard]]
		virtual auto Native() ->ID3D12DescriptorHeap* {
			return _heap.Get();
		}

		virtual auto CpuHandleFor(std::uint32_t index) -> std::shared_ptr<DescriptorHandle> {
			CD3DX12_CPU_DESCRIPTOR_HANDLE handle(_heap->GetCPUDescriptorHandleForHeapStart(), index, _increment);
			auto desc = std::make_shared<DescriptorHandleT<CD3DX12_CPU_DESCRIPTOR_HANDLE>>(handle);
			return desc;
		}

		virtual auto GpuHandleFor(std::uint32_t index) ->  std::shared_ptr<DescriptorHandle> {
			assert(_desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
			CD3DX12_GPU_DESCRIPTOR_HANDLE handle(_heap->GetGPUDescriptorHandleForHeapStart(), index, _increment);
			auto desc = std::make_shared<DescriptorHandleT<CD3DX12_GPU_DESCRIPTOR_HANDLE>>(handle);
			return desc;
		}

	protected:
		bool _isShaderVisible;
		size_t _increment;
		D3D12_DESCRIPTOR_HEAP_DESC _desc;
		D3D12_DESCRIPTOR_HEAP_TYPE _type;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _heap;
		CD3DX12_CPU_DESCRIPTOR_HANDLE _heapStartCPU;
		CD3DX12_GPU_DESCRIPTOR_HANDLE _heapStartGPU;
		size_t _size;
	};
}
