#pragma once

#include <cstdint>

namespace kyanite::engine::rendering {
	class Fence {
	public:
		virtual ~Fence() = default;

		virtual auto Signal() -> void = 0;
		virtual auto SetOnCompletion(uint64_t value, void* event) -> void = 0;
		virtual auto GetCompletedValue() -> uint64_t = 0;
	};
}