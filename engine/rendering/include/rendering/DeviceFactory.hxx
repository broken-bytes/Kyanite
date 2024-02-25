#pragma once

#include "RenderBackendType.hxx"
#include "opengl/GlDevice.hxx"

#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace kyanite::engine::rendering {
	class DeviceFactory {
	public:
		static auto CreateDevice(RenderBackendType backend, SDL_Window* window) -> std::shared_ptr<Device> {
			switch (backend)
			{
			case kyanite::engine::rendering::RenderBackendType::OpenGL:
				return std::make_shared<kyanite::engine::rendering::opengl::GlDevice>(window);
			case kyanite::engine::rendering::RenderBackendType::Vulkan:
				break;
			case kyanite::engine::rendering::RenderBackendType::D3D12:
				break;
			default:
				break;
			}

			// If we reach this point, the backend is not supported
			exit(1);
		}
	};
}