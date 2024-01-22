#pragma once

#include <audio/Audio.hxx>
#include <rendering/Renderer.hxx>
#include <shared/NativePointer.hxx>


namespace kyanite::engine {
	struct Engine {
	public:
		Engine() = default;
		NativePointer window;
		std::shared_ptr<rendering::Renderer> renderer;
	};
}