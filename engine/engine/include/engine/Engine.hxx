#pragma once

#include <audio/Audio.hxx>
#include <rendering/Renderer.hxx>
#include <shared/NativePointer.hxx>
#include <core/ILogger.hxx>
#include <assetpackages/IAssetLoader.hxx>

namespace kyanite::engine {
	struct Engine {
	public:
		Engine() = default;
		NativePointer window;
		std::shared_ptr<rendering::Renderer> renderer;
		std::shared_ptr<kyanite::engine::core::ILogger> logger;
		std::shared_ptr<kyanite::engine::assetpackages::IAssetLoader> assetLoader;
	};
}