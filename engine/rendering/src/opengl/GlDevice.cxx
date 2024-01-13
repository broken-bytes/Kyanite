#include "rendering/opengl/GlDevice.hxx"
#include "rendering/opengl/GlCommandList.hxx"
#include "rendering/opengl/GlCommandQueue.hxx"
#include "rendering/opengl/GlFence.hxx"

namespace rendering::opengl {
	GlDevice::GlDevice() {

	}

	GlDevice::~GlDevice() {

	}

	auto GlDevice::Shutdown() -> void {

	}

	auto GlDevice::CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> {
		return std::make_shared<GlCommandList>(type);
	}

	auto GlDevice::CreateCommandQueue(CommandListType type) -> std::shared_ptr<CommandQueue> {
		return std::make_shared<GlCommandQueue>(type);
	}

	auto GlDevice::CreateFence() -> std::shared_ptr<Fence> {
		return std::make_shared<GlFence>();
	}

	auto GlDevice::CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> {
		return nullptr;
	}

	auto GlDevice::CreateRenderTarget() -> std::shared_ptr<RenderTarget> {
		return nullptr;
	}
}