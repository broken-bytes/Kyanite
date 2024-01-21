#pragma once

#include "AudioClip.hxx"

#include <shared/Exported.hxx>

#include <string>

namespace kyanite::engine::audio {
    struct AudioDevice;
    
    EXPORTED auto QueryDevices() -> std::vector<std::string>;
    EXPORTED auto CreateDevice() -> AudioDevice*;
    EXPORTED auto CreateAudioClip(std::string path) -> AudioClip*;
    EXPORTED auto LoadAudioClip(std::string_view path) -> AudioClip;
}
