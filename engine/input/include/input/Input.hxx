#pragma once

#include "InputEvent.hxx"
#include <shared/Events.hxx>

#include <functional>
#include <vector>

namespace input {
    struct InputEvent;

    enum class Trigger {
        LEFT,
        RIGHT
    };

    auto Init() -> void;
    auto Poll() -> void;

    /**
     * @brief Gets the current state of a keyboard button by symId
     * 
     * @param buttonId The id of the button
     * @return uint8_t 0 = None, 1 = Pressed, 2 = Held, 3 = Up
     */
    auto GetKeyboardButtonState(uint32_t buttonId) -> InputState;

    /**
     * @brief Gets the current state of a mouse button by symId
     * 
     * @param buttonId The id of the button
     * @return uint8_t 0 = None, 1 = Pressed, 2 = Held, 3 = Up
     */
    auto GetMouseButtonState(uint8_t buttonId) -> InputState;

     /**
     * @brief Gets the current state of a mouse axis by id
     * 
     * @param axisId The id of the axis
     * @return float
     */
    auto GetMouseAxis(uint8_t axis) -> float;

    /**
     * @brief Gets the current state of a controller button by symId
     *
     * @param buttonId The id of the button
     * @return uint8_t 0 = None, 1 = Pressed, 2 = Held, 3 = Up
     */
    auto GetControllerButtonState(uint8_t controllerId, uint8_t buttonId) -> InputState;


         /**
     * @brief Gets the current state of a controller axis by id
     * 
     * @param axisId The id of the axis
     * @return float
     */
    auto GetControllerAxis(uint8_t controllerId, uint8_t axis) -> float;

    /**
     * @brief Set the Controller Effect to Bow
     * 
     * @param controllerId
     * @param trigger the trigger to use 
     * @param start 
     * @param end 
     * @param snapForce 
     * @return 
     */
    auto SetControllerEffectBow(uint8_t controllerId, Trigger trigger, uint8_t start, uint8_t end, uint8_t strength, uint8_t snapForce) -> void;

    /**
    * @brief Subscribe to input events
    * 
    * @param callback The callback to be called when an input event is triggered
    */
    auto SubscribeToInputEvents(std::function<void(Event*)> callback) -> void;
}
