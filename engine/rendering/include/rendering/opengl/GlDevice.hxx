#pragma once

#include "../Device.hxx"
#include "../Shader.hxx"

#include <SDL2/SDL.h>

#include <memory>

namespace kyanite::engine::rendering::opengl {
	class GlDevice : public Device, public std::enable_shared_from_this<GlDevice> {
	public:
        GlDevice(SDL_Window* window);
		virtual ~GlDevice();
        virtual auto Shutdown() -> void override;

        // Creation work submission and synchronization
        virtual auto CreateGraphicsContext() -> std::unique_ptr<GraphicsContext> override;
        virtual auto CreateImGuiContext() -> std::unique_ptr<ImGuiContext> override;
        virtual auto CreateCommandList(CommandListType type) -> std::shared_ptr<CommandList> override;
        virtual auto CreateCommandQueue(CommandListType type) -> std::shared_ptr<CommandQueue> override;
        virtual auto CreateFence() -> std::shared_ptr<Fence> override;
        virtual auto CreateSwapchain() -> std::unique_ptr<Swapchain> override;

        // Creation of resources
        virtual auto CreateBuffer(uint64_t size) -> std::shared_ptr<Buffer> override;
        virtual auto CreateRenderTarget() -> std::shared_ptr<RenderTarget> override;
        virtual auto CompileShader(const std::string& shaderSource, ShaderType type) -> uint64_t override;
        virtual auto CreateVertexBuffer(const void* data, uint64_t size) -> std::shared_ptr<VertexBuffer> override;
        virtual auto UpdateVertexBuffer(std::shared_ptr<VertexBuffer> buffer, const void* data, uint64_t size) -> void override;
        virtual auto CreateIndexBuffer(const uint32_t* indices, size_t len) -> std::shared_ptr<IndexBuffer> override;
        virtual auto UpdateIndexBuffer(std::shared_ptr<IndexBuffer> buffer, std::vector<uint32_t> indices) -> void override;

        //Delete resources
        virtual auto DestroyShader(uint64_t shaderHandle) -> void override;

    private:
        SDL_Window* _window;
        SDL_GLContext _glContext;
	};
}