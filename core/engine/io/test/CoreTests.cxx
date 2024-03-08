#include <GTest/GTest.h>

#include <Core.hxx>

TEST(CoreBridge, TestShowWindow) {
    try {
        auto window = Bridge_Core_CreateWindow("Test", 0, 0, 800, 600, 0);
        Bridge_Core_ShowWindow(window);
    } catch(std::exception error) {
        FAIL();
    }
}
