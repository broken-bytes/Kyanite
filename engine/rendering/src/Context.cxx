#include "rendering/Context.hxx"

#include "rendering/Device.hxx"

namespace kyanite::engine::rendering {
	Context::Context(CommandListType type, const std::shared_ptr<Device>& device) : _type(type), _device(device) {
		_commandList = _device->CreateCommandList(type);
	}
}