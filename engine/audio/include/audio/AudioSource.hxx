#pragma once

#include <stdint.h>

namespace audio {
    struct AudioClip;

    class AudioSource {
    public:
        AudioSource(float x, float y, float z);
        ~AudioSource();
        
        auto PlayAudioClip(AudioClip* clip) -> void;
        auto Start() -> void;
        auto Stop() -> void;
        auto Tick() -> void;
        auto SetClip(AudioClip* clip) -> void;
        auto SetPosition(float x, float y, float z) -> void;
        auto SetRotation(float x, float y, float z) -> void;
        auto SetPitch(float value) -> void;
        auto SetGain(float value) -> void;
        auto SetLooping(bool value) -> void;
        
    private:
        uint64_t _id;
    };
}
