#include <d3d12/D3D12CommandQueue.hxx>
#include <stdexcept>
#include <d3d12/D3D12Fence.hxx>
#include <d3d12/D3D12GraphicsCommandList.hxx>
#include "d3d12/Utils.hxx"
#include <sstream>
#include <string>
#include <Core/Logger.hxx>

namespace Renderer {
    D3D12CommandQueue::D3D12CommandQueue(CommandType type, D3D12Device& device, std::string name): CommandQueue(type, name) {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE(type);

        auto native = device.Native();
        std::wstring newName;
        Utils::ConvertStrToWstr(newName, name);

        if (S_OK != device.Native()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_queue))) {
            throw std::runtime_error("Failed to create command queue");
        }

        _queue->SetName(newName.c_str());

    }

    auto D3D12CommandQueue::ExecuteCommandLists(std::vector<std::shared_ptr<GraphicsCommandList>> lists) -> void {
        std::vector<ID3D12CommandList*> dxList;
        std::stringstream str;
        str << "Executing Commandlists on Queue " << Name();
        Logger::Info(str.str());

        for (const auto& item : lists) {
            dxList.push_back(static_pointer_cast<D3D12GraphicsCommandList>(item)->Native());
        }
        _queue->ExecuteCommandLists(dxList.size(), dxList.data());
    }

    auto D3D12CommandQueue::Signal(std::shared_ptr<Fence> fence, uint64_t value)->uint8_t {
        std::stringstream str;
        str << "Signaling Queue " << Name() << " with value " << value;
        Logger::Info(str.str());
        auto d3dFence = static_pointer_cast<D3D12Fence>(fence)->Native();
        _queue->Signal(d3dFence, value);
        return 0;
    }

    auto D3D12CommandQueue::Native()->ID3D12CommandQueue* {
        return _queue.Get();
    }
}