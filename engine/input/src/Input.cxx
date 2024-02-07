#include "input/Input.hxx"
#include "input/InputEvent.hxx"
#include "input/DualsenseTriggerEffectGenerator.hxx"

#include <math/Utils.hxx>
#include <shared/Events.hxx>

#include <SDL2/SDL.h>
#include "atomic_queue/atomic_queue.h"

#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <Windows.h>

namespace input {
    bool quit = false;
    std::vector<std::function<void(Event*)>> inputEventSubscribers;
    // 0 = Last Frame, 1 = Current Frame
    std::array<std::map<uint32_t, InputState>, 2> KeyboardButtonState;
    std::array<std::map<uint32_t, InputState>, 2> MouseButtonState;
    // 0 = Movement, 1 = Pos
    std::array<std::map<uint8_t, float>, 2> MouseAxisState;
    std::array<std::map<uint8_t, int32_t>, 2> MousePositionState;

    std::map<uint8_t, SDL_GameController*> Controllers = {};
    std::map<uint8_t, std::array<std::map<uint32_t, InputState>, 2>> ControllerButtonState = {};
    
    void ProcessInputs();
    void ProcessInputFor(std::array<std::map<uint32_t, InputState>, 2>& map);
    void ProcessKeyboardInput();
    void ProcessControllerInput();
    void ProcessMouseInput();
    
    auto Init() -> void {
        SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC );
        SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    }

    auto Poll() -> void {
        SDL_Event event = {};
        
        // Make Current frame last frame
        KeyboardButtonState[0] = KeyboardButtonState[1];
        for(int x = 0; x < ControllerButtonState.size(); x++) {
            ControllerButtonState[x][0] = ControllerButtonState[x][1];
        }
        MouseButtonState[0] = MouseButtonState[1];
        MouseAxisState[0] = MouseAxisState[1];
        MousePositionState[0] = MousePositionState[1];
        // Create new frame
        KeyboardButtonState[1] = {};
        for(int x = 0; x < ControllerButtonState.size(); x++) {
            ControllerButtonState[x][1] = {};
        }
        MouseButtonState[1] = {};
        MousePositionState[1] = {};
        MouseAxisState[1] = {};
        
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                SystemEvent quitEvent = {};
                quitEvent.type = SystemEventType::Quit;

                Event* event = new Event();
                event->type = EventType::System;
                event->data.system = quitEvent;

                for(auto& subscriber: inputEventSubscribers) {
					subscriber(event);
				}
            }
            
            if(event.type == SDL_KEYDOWN) {
                KeyboardButtonState[1][event.key.keysym.scancode] = InputState::PRESSED;

                KeyEvent keyEvent = {};
                keyEvent.type = KeyEventType::KeyPressed;
                keyEvent.scancode = event.key.keysym.scancode;

                Event* event = new Event();
                event->type = EventType::Key;
                event->data.key = keyEvent;

                for (auto& subscriber : inputEventSubscribers) {
                    subscriber(event);
                }
            }
            
            if(event.type == SDL_KEYUP) {
                KeyboardButtonState[1][event.key.keysym.scancode] = InputState::RELEASED;

                KeyEvent keyEvent = {};
                keyEvent.type = KeyEventType::KeyReleased;
                keyEvent.scancode = event.key.keysym.scancode;

                Event* event = new Event();
                event->type = EventType::Key;
                event->data.key = keyEvent;

                for (auto& subscriber : inputEventSubscribers) {
                    subscriber(event);
                }
            }
            
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                MouseButtonState[1][event.button.button] = InputState::PRESSED;
            }
            
            if(event.type == SDL_MOUSEBUTTONUP) {
                MouseButtonState[1][event.button.button] = InputState::RELEASED;
            }
            
            if(event.type == SDL_MOUSEMOTION) {
                auto window = SDL_GetWindowFromID(event.motion.windowID);
                int w = 0;
                int h = 0;
                SDL_GetWindowSize(window, &w, &h);
                MouseAxisState[1][0] = (float)event.motion.xrel / (float)w;
                MouseAxisState[1][1] = (float)event.motion.yrel / (float)h;
                MousePositionState[1][0] = event.motion.x;
                MousePositionState[1][1] = event.motion.y;
            }
            
            if(event.type == SDL_CONTROLLERBUTTONDOWN) {
                ControllerButtonState[event.cbutton.which][1][event.cbutton.button] = InputState::PRESSED;
            }
            
            if(event.type == SDL_CONTROLLERBUTTONUP) {
                ControllerButtonState[event.cbutton.which][1][event.cbutton.button] = InputState::RELEASED;
            }
            
            if(event.type == SDL_CONTROLLERAXISMOTION) {
                // Do not process for now, as SDL_ControllerAxisGet does the same without having to hold another array
            }
            
            if(event.type == SDL_JOYDEVICEADDED) {
                Controllers[event.jdevice.which] = SDL_GameControllerOpen(event.jdevice.which);
            }
            
            if(event.type == SDL_JOYDEVICEREMOVED) {
                SDL_GameControllerClose(Controllers[event.jdevice.which]);
            }
        }
        
        ProcessInputs();
    }

    auto GetKeyboardButtonState(uint32_t buttonId) -> InputState {
        return KeyboardButtonState[1][buttonId];
    }

    auto GetMouseButtonState(uint8_t buttonId) -> InputState {
        return MouseButtonState[1][buttonId];
    }

    auto GetMouseAxis(uint8_t axis) -> float {
        return MouseAxisState[1][axis];
    }

    auto GetControllerButtonState(uint8_t controllerId, uint8_t buttonId) -> InputState {
        return ControllerButtonState[controllerId][1][buttonId];
    }

    auto GetControllerAxis(uint8_t controllerId, uint8_t axis) -> float {
        return math::utils::ToNormalizedFloat(SDL_GameControllerGetAxis(Controllers[controllerId], SDL_GameControllerAxis(axis)));
    }

    // -- Controller Effects --

    auto SetControllerEffectBow(uint8_t controllerId, Trigger trigger, uint8_t start, uint8_t end, uint8_t strength, uint8_t snapForce) -> void {
        std::vector<uint8_t> data(48);
        data[0] = 0x02 | (0x04 | 0x08);
        data[1] = 0xFF;
        data[2] = 0x15;
        input::dualsense::TriggerEffectGenerator::Bow(data, trigger == Trigger::LEFT ? 21 : 10, start, end, strength, snapForce);
        SDL_GameControllerSendEffect(Controllers[controllerId], data.data(), data.size());
    }

    auto SubscribeToInputEvents(std::function<void(Event*)> callback) -> void {
        inputEventSubscribers.push_back(callback);
    }

    // ----- Helper Methods -----
    void ProcessInputs() {
        ProcessKeyboardInput();
        ProcessControllerInput();
        ProcessMouseInput();
    }

    void ProcessInputFor(std::array<std::map<uint32_t, InputState>, 2>& map) {
        for(int x = 0; x < map[0].size(); x++) {
            if(
               map[0][x] == InputState::PRESSED ||
               map[0][x] == InputState::HELD
            ) {
                if(map[1][x] == InputState::NONE) {
                    map[1][x] = InputState::HELD;
                }
            }
        }
    }

    void ProcessKeyboardInput() {
        ProcessInputFor(KeyboardButtonState);
    }

    void ProcessControllerInput() {
        for(auto& item: ControllerButtonState) {
            ProcessInputFor(item.second);
        }
    }

    void ProcessMouseInput() {
        ProcessInputFor(MouseButtonState);
    }
}
