#pragma once

#include <cstdint>

namespace Renderer {
    class Fence {
        public:
        friend class Device;


		Fence(std::uint64_t value) {
                FenceValue = value;
        }

		virtual auto SetValue(std::uint64_t value) -> void {
            FenceValue = value;
        }

		virtual auto Increment() -> std::uint64_t {
            FenceValue++;
		    return FenceValue;
        }

		[[nodiscard]] virtual auto GetCompletedValue()-> std::uint64_t = 0;
		virtual auto SetOnCompletion(std::uint64_t value, void* event) -> void = 0;
		virtual auto Wait(std::uint64_t value, void* event) -> void = 0;

        private:
        	std::uint64_t FenceValue = 0;
	};
}

