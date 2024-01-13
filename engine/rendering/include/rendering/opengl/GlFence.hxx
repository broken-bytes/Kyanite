#pragma once

#include "../Fence.hxx"

namespace rendering::opengl {
	class GlFence : public Fence {
	public:
		GlFence();
		~GlFence();

		virtual auto Signal() -> void override;
		virtual auto SetOnCompletion(uint64_t value, void* event) -> void override;
		virtual auto GetCompletedValue() -> uint64_t override;

	private:
		uint64_t _value;
	};
}