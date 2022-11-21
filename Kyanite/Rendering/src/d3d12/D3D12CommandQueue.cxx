#include <d3d12/D3D12CommandQueue.hxx>
#include <stdexcept>
#include <d3d12/D3D12Fence.hxx>
#include <d3d12/D3D12GraphicsCommandList.hxx>

namespace Renderer {
    D3D12CommandQueue::D3D12CommandQueue(CommandType type, D3D12Device& device): CommandQueue(type) {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE(type);

        auto native = device.Native();

        if (S_OK != device.Native()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_queue))) {
            throw std::runtime_error("Failed to create command queue");
        }
    }

    auto D3D12CommandQueue::ExecuteCommandLists(std::vector<std::shared_ptr<GraphicsCommandList>> lists) -> void {
        std::vector<ID3D12CommandList*> dxList;

        for (const auto& item : lists) {
            dxList.push_back(static_pointer_cast<D3D12GraphicsCommandList>(item)->Native());
        }
        _queue->ExecuteCommandLists(dxList.size(), dxList.data());
    }

    auto D3D12CommandQueue::Signal(std::shared_ptr<Fence> fence, uint64_t value)->uint8_t {
        auto d3dFence = static_pointer_cast<D3D12Fence>(fence)->Native();
        _queue->Signal(d3dFence, value);
        return 0;
    }

    auto D3D12CommandQueue::Native()->ID3D12CommandQueue* {
        return _queue.Get();
    }
}