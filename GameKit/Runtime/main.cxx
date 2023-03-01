#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef __APPLE__
#include <dlfcn.h>
#endif
#include <iostream>

int main(int argc, char** argv) {
#ifdef _WIN32
    auto lib = LoadLibraryA("./Assembly.dll");
    std::cout << "Lib is |" << ((lib != nullptr) ? "Found " : "Not found ") << "|" << std::endl;
    
    auto start = GetProcAddress(lib, "GameKitMain");
#endif
#ifdef __APPLE__
    auto lib = dlopen("./Assembly", RTLD_LOCAL);
    auto start = (void (*)())dlsym(lib, "GameKitMain");
#endif
    start();

    return 0;
}
