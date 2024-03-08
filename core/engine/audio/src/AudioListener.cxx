#include "audio/AudioListener.hxx"

#include <AL/al.h>

namespace kyanite::engine::audio {
    AudioListener::AudioListener() {
        SetPosition(0, 0, 1.0f);
        SetRotation(0, 0, 0);
    }

    AudioListener::~AudioListener() {

    }

    auto AudioListener::SetPosition(float x, float y, float z) -> void {
        alListener3f(AL_POSITION, x, y, z);
    }

    auto AudioListener::SetRotation(float x, float y, float z) -> void {
        alListener3f(AL_ORIENTATION, x, y, z);
    }
}
