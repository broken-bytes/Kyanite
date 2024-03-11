#include "system/Bridge_System.h"

#include <SDL2/SDL.h>

void* System_CreateWindow(
    const char* title,
    int32_t posX,
    int32_t posY,
    int32_t width,
    int32_t height
) {
	return SDL_CreateWindow(title, posX, posY, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
}