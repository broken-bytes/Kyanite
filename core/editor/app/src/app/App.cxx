#include "app/App.hxx"

#include <filesystem>
#include <memory>
#include <sstream>

#pragma comment(lib, "KyaniteEngine.lib")
#pragma comment(lib, "KyaniteEditor.lib")

// NOTE: These functions come from the Swift libraries. They are *not* unused 
extern "C" void kyanitemain(bool);
extern "C" void kyaniteeditormain();

int main(int argc, char** argv) {
	kyaniteeditormain();
}