#include "rendering/Context.hxx"

#include "rendering/Device.hxx"

namespace kyanite::engine::rendering {
	Context::Context(
		CommandListType type, 
		const std::shared_ptr<Device>& device,
		std::shared_ptr<CommandQueue> queue
	) : _type(type), _device(device), _commandQueue(queue) {
		_commandList = _device->CreateCommandList(type);
		_commandAllocator = _device->CreateCommandAllocator();
	}
}