#include "core/Core.hxx"

#include <SDL2/SDL.h>
#include <SDL2/SDL_Vulkan.h>

namespace core {
    auto InitCore() -> void {
        SDL_Init(SDL_INIT_EVENTS);
    }
}
