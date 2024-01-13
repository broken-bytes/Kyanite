#pragma once

#include "../Device.hxx"

namespace rendering::opengl {
	class GlDevice : public Device {
	public:
        GlDevice();
		virtual ~GlDevice();
        virtual auto Shutdown() -> void override;

        // Creation work submission and synchronization

        virtual auto CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> override;
        virtual auto CreateCommandQueue(CommandListType type) -> std::shared_ptr<CommandQueue> override;
        virtual auto CreateFence() -> std::shared_ptr<Fence> override;

        // Creation of resources
        virtual auto CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> override;
        virtual auto CreateRenderTarget() -> std::shared_ptr<RenderTarget> override;
	};
}