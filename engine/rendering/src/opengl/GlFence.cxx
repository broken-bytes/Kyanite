#include "rendering/opengl/GlFence.hxx"

namespace rendering::opengl {
	GlFence::GlFence() {
		_value = 0;
	}

	GlFence::~GlFence() {

	}

	auto GlFence::Signal() -> void {
		_value++;
	}

	auto GlFence::SetOnCompletion(uint64_t value, void* event) -> void {
		// NOTE: This is a NOP on OpenGL
	}

	auto GlFence::GetCompletedValue() -> uint64_t {
		return _value;
	}
}