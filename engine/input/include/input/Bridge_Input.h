#pragma once

#include "Input.hxx"
#include "InputEvent.hxx"

#include <core/Exported.hxx>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif

/**
 * @brief Inits the input system
 *
 */
EXPORTED void Bridge_Iput_Init() {
    input::Init();
}

/**
 * @brief Polls from the input system
 * 
 */
EXPORTED void Bridge_Input_Poll() {
    input::Poll();
}

/**
 * @brief Gets the state of the given keyboard button
 * @param buttonId The Id of the button to query
 *
 */
EXPORTED uint8_t Bridge_Input_GetKeyboardButtonState(uint32_t buttonId) {
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

/**
 * @brief Gets the state of the given mouse button
 * @param buttonId The Id of the button to query
 *
 */
EXPORTED uint8_t Bridge_Input_GetMouseButtonState(uint32_t buttonId) {
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

/**
 * @brief Gets the value of the given mouse axis
 * @param buttonId The Id of the button to query
 *
 */
EXPORTED float Bridge_Input_GetMouseAxis(uint8_t axis) {
    return input::GetMouseAxis(axis);
}

/**
 * @brief Gets the state of the given controller button
 * @param uint8_t The controller to be used
 * @param buttonId The Id of the button to query
 *
 */
EXPORTED uint8_t Bridge_Input_GetControllerButtonState(uint8_t controllerId, uint8_t buttonId) {
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

/**
 * @brief Gets the value of the given controller axis
 * @param uint8_t The controller to be used
 * @param buttonId The Id of the button to query
 *
 */
EXPORTED float Bridge_Input_GetControllerAxis(uint8_t controllerId, uint8_t axis) {
    return input::GetControllerAxis(controllerId, axis);
}

#ifdef __cplusplus 
}
#endif
