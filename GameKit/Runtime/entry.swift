/*#include <Windows.h>
#include <iostream>

int main(int argc, char** argv) {
    auto lib = LoadLibraryA("./Assembly.dll");

    std::cout << "Lib is |" << ((lib != nullptr) ? "Found " : "Not found ") << "|" << std::endl;
    
    auto start = GetProcAddress(lib, "GameKitMain");

    start();

    return 0;
}

*/

@main
struct App {
    static var bridge: Bridge!

    public static func main() -> Void {
        bridge = Bridge()
    }
}