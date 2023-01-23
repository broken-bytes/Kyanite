#pragma once

#include <cstdint>
#include <SDL2/SDL.h>
#include <vector>

namespace InputHandler {
    enum class MouseButton: uint8_t {
        Left = 1,
        Right = 3,
        Middle = 2,
        Extra0 = 4,
        Extra1 = 5
    };

    enum class ButtonState {
        Up = 0, // Button was not pressed last frame
        Down = 1, // Button was pressed this frame
        Held = 2, // Button was already pressed last frame and is still being pressed
        Released = 3 // The button was held last frame but is now released
    };

    enum class InputEventType {

    };


    auto Init() -> void;

    auto AddMouseClickEvent(MouseButton mouseButton, ButtonState state) -> void;
    auto AddMouseMoveEvent(uint32_t x, uint32_t y) -> void;
    auto AddKeyboardEvent(SDL_Scancode code, ButtonState state) -> void;

    auto GetKeyboardButtonState(SDL_Scancode code) -> ButtonState;
    auto GetMouseButtonState(MouseButton button) -> ButtonState;
    auto GetMouseMovement() -> std::vector<uint32_t>;
    auto GetMouseAbs() -> std::vector<uint32_t>;
}

