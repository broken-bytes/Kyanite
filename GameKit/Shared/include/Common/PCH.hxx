#pragma once

#ifdef __swift__
#include <stdint.h>
#include <stdlib.h>
#else
#include <cstdint>
#include <malloc.h>
#endif

struct SDL_Window;

typedef void* NativeEvent;
typedef SDL_Window* Window;