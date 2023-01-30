#pragma once

#include <GraphicsCommandList.hxx>
#include <CommandQueue.hxx>
#include <d3d12.h>
#include <wrl.h>
#include <d3d12/D3D12Device.hxx>

namespace Renderer {
	class D3D12CommandQueue : public CommandQueue {
    public:
        D3D12CommandQueue(CommandType type, D3D12Device& device, std::string name);
        virtual auto ExecuteCommandLists(std::vector<std::shared_ptr<GraphicsCommandList>> lists) -> void;
        [[nodiscard]] virtual auto Signal(std::shared_ptr<Fence> fence, uint64_t value)->uint8_t;
        [[nodiscard]] auto Native()->ID3D12CommandQueue*;
    private:
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> _queue;
	};
}