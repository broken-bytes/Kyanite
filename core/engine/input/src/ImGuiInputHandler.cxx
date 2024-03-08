#include "input/ImGuiInputHandler.hxx"

#include "input/Input.hxx"

#include "imgui.h"

namespace kyanite::engine::input {
	ImGuiInputHandler::ImGuiInputHandler() {
		input::SubscribeToInputEvents([this](Event* event) {
			//this->HandleInputEvent(event);
			});
	}

	auto ImGuiInputHandler::HandleInputEvent(Event* event) -> void {
		switch (event->type) {
		case EventType::Key:
			HandleKeyEvent(&event->data.key);
			break;
		case EventType::Mouse:
			//HandleMouseEvent(&event->data);
			break;
		}
	}

	auto ImGuiInputHandler::HandleKeyEvent(KeyEvent* event) -> void {
		ImGuiIO& io = ImGui::GetIO();
		switch (event->type) {
		case KeyEventType::KeyPressed:
			io.KeysDown[event->scancode] = true;
			break;
		case KeyEventType::KeyReleased:
			io.KeysDown[event->scancode] = false;
			break;
		}
	}

	auto ImGuiInputHandler::HandleMouseEvent(MouseEvent* event) -> void {
		ImGuiIO& io = ImGui::GetIO();
		switch (event->type) {
		case MouseEventType::MouseButtonPressed:
			io.MouseDown[event->button] = true;
			break;
		case MouseEventType::MouseButtonReleased:
			io.MouseDown[event->button] = false;
			break;
		case MouseEventType::MouseMoved:
			io.MousePos = ImVec2(event->x, event->y);
			break;
		case MouseEventType::MouseScrolled:
			io.MouseWheel = event->y;
			break;
		}
	}
}