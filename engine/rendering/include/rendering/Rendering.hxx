#pragma once

#include <core/NativePointer.hxx>

namespace rendering {
	auto Init(NativePointer window) -> void;
	auto Update() -> void;
	auto Render() -> void;
	auto SetClearColor(float r, float g, float b, float a) -> void;
}