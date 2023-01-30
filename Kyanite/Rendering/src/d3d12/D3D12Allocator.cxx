#include <d3d12/D3D12Allocator.hxx>
#include <Core/Logger.hxx>
#include <string>
#include <sstream>

namespace Renderer {
	D3D12Allocator::D3D12Allocator(CommandType type, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator, std::string name) : Allocator(type, name) {
		_allocator = allocator;
	}

	auto D3D12Allocator::Reset() -> void {
		std::stringstream str;
		str << "Resetting Allocator " << Name();
		Logger::Info(str.str());
		_allocator->Reset();
	}

	auto D3D12Allocator::Native()->ID3D12CommandAllocator* {
		return _allocator.Get();
	}
}
