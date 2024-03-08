#pragma once

#include "input/Input.hxx"

namespace kyanite::engine::input {
	class ImGuiInputHandler {
	public:
		ImGuiInputHandler();

	private:
		auto HandleInputEvent(Event* event) -> void;
		auto HandleKeyEvent(KeyEvent* event) -> void;
		auto HandleMouseEvent(MouseEvent* event) -> void;
	};
}