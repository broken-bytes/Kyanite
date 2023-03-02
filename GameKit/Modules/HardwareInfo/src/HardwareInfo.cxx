#include "HardwareInfo.hxx"

#include <thread>

uint8_t NumCores() {
    return std::thread::hardware_concurrency();
}
