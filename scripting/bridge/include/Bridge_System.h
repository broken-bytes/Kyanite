#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#ifdef __cplusplus 
extern "C" {
#endif
	EXPORTED void* System_CreateWindow(
        const char* title,
        int32_t posX,
        int32_t posY,
        int32_t width,
        int32_t height
    );
#ifdef __cplusplus 
}
#endif
