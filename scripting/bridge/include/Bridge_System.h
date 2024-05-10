#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#ifdef __cplusplus 
extern "C" {
#endif
	EXPORTED void* System_CreateWindow(const char* title, int posX, int posY, int width, int height);
#ifdef __cplusplus 
}
#endif
