#include <audio/Bridge_Audio.h>
#include <core/Bridge_Core.h>
#include <input/Bridge_Input.h>
//#include <rendering/Bridge_Rendering.hxx>

#include <stddef.h>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <signal.h>

int WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
) {
	Bridge_Core_Init();
    Bridge_Iput_Init();
    auto window = Bridge_Core_CreateWindow("Test", 0, 0, 800, 600, 0, 0, false);
    Bridge_Core_ShowWindow(window);
    //Bridge_Rendering_Init(window);
    auto device = Bridge_Audio_CreateDevice();
    Bridge_Audio_DestroyDevice(device);


    while(true) {
        Bridge_Input_Poll();
        auto input = Bridge_Input_GetMouseAxis(0);
        //Bridge_Rendering_Render();
 
    }

}
