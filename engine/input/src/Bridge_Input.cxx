#pragma once

#include "input/Bridge_Input.h"
#include "input/Input.hxx"

#include <shared/Exported.hxx>

#include <stdint.h>
#include <stddef.h>

void Bridge_Iput_Init() {
    input::Init();
}

void Bridge_Input_Poll() {
    input::Poll();
}

uint8_t Bridge_Input_GetKeyboardButtonState(uint32_t buttonId) {
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

uint8_t Bridge_Input_GetMouseButtonState(uint32_t buttonId) {
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

float Bridge_Input_GetMouseAxis(uint8_t axis) {
    return input::GetMouseAxis(axis);
}

uint8_t Bridge_Input_GetControllerButtonState(uint8_t controllerId, uint8_t buttonId) {
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

float Bridge_Input_GetControllerAxis(uint8_t controllerId, uint8_t axis) {
    return input::GetControllerAxis(controllerId, axis);
}

