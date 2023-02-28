#include "WindowManager.hxx"

#include <SDL_video.h>
#include <SDL.h>

namespace WindowManager {
    
    auto Initialise() -> int {
        return SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS);
    }

    auto CreateWindow(WindowCreationDesc desc) -> Window {
        int windowFlags = 0;

        if(desc.resizable) {
            windowFlags |=  SDL_WindowFlags::SDL_WINDOW_RESIZABLE;
        }

        if(desc.fullscreen) {
            windowFlags |= SDL_WindowFlags::SDL_WINDOW_FULLSCREEN;
        }

        return SDL_CreateWindow(desc.name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, desc.width, desc.height, windowFlags);
    }

    auto HandleWindowEvents() -> void {
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                break;
            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
                break;
            default:
                break;
            }
        }
    }
}