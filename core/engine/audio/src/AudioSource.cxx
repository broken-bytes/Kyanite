#include "audio/AudioSource.hxx"
#include "audio/AudioClip.hxx"

#include <al/al.h>

#include <iostream>

namespace kyanite::engine::audio {
    AudioSource::AudioSource(float x, float y, float z) {
        ALuint source;
        alGenSources(1, &source);
        _id = source;
        alSourcei(_id, AL_BUFFER, AL_NONE);
                
        SetPosition(x, y, z);
        SetLooping(false);
        SetPitch(1.0);
        SetGain(1.0);
    }

    AudioSource::~AudioSource() {
        alDeleteSources(1, reinterpret_cast<ALuint*>(&_id));
    }
    
    auto AudioSource::PlayAudioClip(AudioClip* clip) -> void {
        SetClip(clip);
        Start();
        SetLooping(false);
    }

    auto AudioSource::Start() -> void {
        alSourcePlay(_id);
    }

    auto AudioSource::Stop() -> void {
        alSourceStop(_id);
    }

    auto AudioSource::Tick() -> void {
        ALint state = 0;
        alGetSourcei(_id, AL_SOURCE_STATE, &state);
    }

    auto AudioSource::SetClip(AudioClip* clip) -> void {
        auto buff = clip->BufferId();
        alSourcei(_id, AL_BUFFER, buff);
    }

    auto AudioSource::SetPosition(float x, float y, float z) -> void {
        alSource3f(_id, AL_POSITION, x, y, z);
    }

    auto AudioSource::SetRotation(float x, float y, float z) -> void {
        alSource3f(_id, AL_ORIENTATION, x, y, z);
    }

    auto AudioSource::SetPitch(float value) -> void {
        alSourcef(_id, AL_PITCH, value);
    }

    auto AudioSource::SetGain(float value) -> void {
        alSourcef(_id, AL_GAIN, value);
    }

    auto AudioSource::SetLooping(bool value) -> void {
        alSourcei(_id, AL_LOOPING, value);
    }
}
