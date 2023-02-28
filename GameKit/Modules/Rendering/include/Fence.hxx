#pragma once

#include "Common/PCH.hxx"

namespace Rendering {

    class Fence {

        public:
        Fence(uint64_t value) : _fenceValue(value) {};
        virtual auto SetValue(uint64_t value) -> void = 0;
        [[nodiscard("Storing the new fence value is good practice")]]  
        virtual auto Increment() -> uint64_t = 0;
        [[nodiscard("You shouldn't call GetCompletedValue for no reason")]]
        [[nodiscard]] virtual auto GetCompletedValue()-> std::uint64_t = 0;
		virtual auto SetOnCompletion(uint64_t value, NativeEvent event) -> void = 0;
		virtual auto Wait(uint64_t value, NativeEvent event) -> void = 0;

        protected:
        uint64_t _fenceValue;
    };
}