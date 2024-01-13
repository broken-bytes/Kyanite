#pragma once

#include <SDL2/SDL.h>

namespace input {
    class Controller {
        public:
        Controller(SDL_GameController* controller);

        private:
        SDL_GameController* _controller;
    };
}