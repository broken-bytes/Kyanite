#include "Device.hxx"
#include "d3d12/D3D12Adapter.hxx"
#include "d3d12/D3D12Device.hxx"

#include <directx/d3d12.h>
#include <SDL.h>

#include <stdexcept>

namespace Rendering {
        
    D3D12Device::D3D12Device(Window window): Device(window) {
        uint32_t dxgiFactoryFlags = 0;
#if NDEBUG
        Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
#endif

    auto adapter = D3D12Adapter::Adapters()[0];

    if (SUCCEEDED(
        D3D12CreateDevice(
            adapter->Raw(),
            D3D_FEATURE_LEVEL_12_1,
            __uuidof(ID3D12Device9), 
            &_device)
        )
    ) {
        OutputDebugStringW(L"Created device \n");
#ifdef NDEBUG
    ID3D12Debug *pdx12Debug = NULL;
    if (SUCCEEDED(
        D3D12GetDebugInterface(
            IID_PPV_ARGS(&pdx12Debug)
        ))
    ) {
        pdx12Debug->EnableDebugLayer();
    }
    if (pdx12Debug != NULL) {
        ID3D12InfoQueue *pInfoQueue = NULL;
        _device->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_INFO, true);
        pInfoQueue->Release();
        pdx12Debug->Release();
    }
#endif
        } else {
            throw std::runtime_error("Failed creating device");
        }
    }

    D3D12Device::~D3D12Device() {

    }

    auto D3D12Device::CreateSwapchain(uint8_t numBuffers, uint32_t width, uint32_t height) -> Swapchain {

    }
    
    auto D3D12Device::CreateVertexBuffer() -> Buffer {

    }
    
    auto D3D12Device::CreateIndexBuffer() -> Buffer {

    }
    
    auto D3D12Device::GetList(CommandList::CommandListType type, uint8_t index) -> CommandList {

    }
    
    auto D3D12Device::GetQueue(CommandList::CommandListType type) -> CommandQueue {

    }
}