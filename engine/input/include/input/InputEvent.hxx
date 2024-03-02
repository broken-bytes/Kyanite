#pragma once

#include <stdint.h>

namespace kyanite::engine::input {
    enum class InputState : uint8_t {
        NONE = 0,
        PRESSED = 1,
        HELD = 2,
        RELEASED = 3
    };

    struct MouseInput {
        InputState State;
        uint8_t ButtonId;
    };

    struct MouseAxis {
        uint8_t AxisId;
        float Value;
    };

    struct ControllerInput {
        InputState State;
        uint8_t ControllerId;
        uint8_t ButtonId;
    };

    struct ControllerAxis {
        uint8_t ControllerId;
        uint8_t AxisId;
        float Value;
    };

    struct KeyboardInput {
        InputState State;
        uint32_t KeyCode;
    };

    enum InputType {
        MOUSE_BUTTON,
        MOUSE_AXIS,
        KEYBOARD_BUTTON,
        CONTROLLER_BUTTON,
        CONTROLLER_AXIS
    };

    struct InputEvent {
        InputType Type;
        union Data {
            MouseInput MouseInput;
            MouseAxis MouseAxis;
            KeyboardInput KeyboardInput;
            ControllerInput ControllerInput;
            ControllerAxis ControllerAxis;
        } Data;
    };
}
