#pragma once

#include "input/Bridge_Input.h"
#include "input/Input.hxx"

#include <shared/Exported.hxx>
#include <imgui.h>

#include <stdint.h>
#include <stddef.h>

namespace input = kyanite::engine::input;

void Input_Init(NativePointer imGuiContext) {
    kyanite::engine::input::Init(reinterpret_cast<ImGuiContext*>(imGuiContext));
}

void Input_Poll() {
    input::Poll();
}

uint8_t Input_GetKeyboardButtonState(uint32_t buttonId) {
    switch(input::GetKeyboardButtonState(buttonId)) {
        case input::InputState::NONE:
            return 0;
        case input::InputState::PRESSED:
            return 1;
        case input::InputState::HELD:
            return 2;
        case input::InputState::RELEASED:
            return 3;
    }
}

uint8_t Input_GetMouseButtonState(uint32_t buttonId) {
    switch(input::GetMouseButtonState(buttonId)) {
        case input::InputState::NONE:
            return 0;
        case input::InputState::PRESSED:
            return 1;
        case input::InputState::HELD:
            return 2;
        case input::InputState::RELEASED:
            return 3;
    }
}

float Input_GetMouseAxis(uint8_t axis) {
    return input::GetMouseAxis(axis);
}

uint8_t Input_GetControllerButtonState(uint8_t controllerId, uint8_t buttonId) {
    switch(input::GetControllerButtonState(controllerId, buttonId)) {
        case input::InputState::NONE:
            return 0;
        case input::InputState::PRESSED:
            return 1;
        case input::InputState::HELD:
            return 2;
        case input::InputState::RELEASED:
            return 3;
    }

    return 0;
}

float Input_GetControllerAxis(uint8_t controllerId, uint8_t axis) {
    return input::GetControllerAxis(controllerId, axis);
}

void Input_Subscribe(void(*subscription)(Event* event)) {
	input::SubscribeToInputEvents(subscription);
}

