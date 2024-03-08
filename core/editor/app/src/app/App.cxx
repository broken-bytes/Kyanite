#include "app/App.hxx"

#include <SDL2/SDL.h>

#include <filesystem>
#include <memory>
#include <sstream>

#pragma comment(lib, "KyaniteEngine.lib")
#pragma comment(lib, "KyaniteEditor.lib")

// NOTE: These functions come from the Swift libraries. They are *not* unused 
extern "C" void kyanitemain(bool);
extern "C" void kyaniteeditormain(void* window);

int main(int argc, char** argv) {
	auto window = SDL_CreateWindow(
		"Kyanite", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		800, 
		600, 
		SDL_WINDOW_OPENGL
	);
	kyaniteeditormain(window);	

	return 0;
}