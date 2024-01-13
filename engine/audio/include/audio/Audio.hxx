#pragma once

#include "AudioClip.hxx"

#include <string>

namespace audio {
    struct AudioDevice;
    
    auto QueryDevices() -> std::vector<std::string>;
    auto CreateDevice() -> AudioDevice*;
    auto CreateAudioClip(std::string path) -> AudioClip*;
}
