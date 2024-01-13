#include "audio/AudioClip.hxx"
#include <al/alut.h>

#include <stdint.h>
#include <iostream>

namespace audio {
    AudioClip::AudioClip(
        std::vector<float> buffer,
        int sampleRate, 
        int bitDepth, 
        int numSamples, 
        int numChannels, 
        float length, 
        bool stereo
    ) : _buffer(buffer), 
        _sampleRate(sampleRate), 
        _bitDepth(bitDepth),
        _numSamples(numSamples), 
        _numChannels(numChannels),
        _length(length),
        _stereo(stereo)
    {
        alGenBuffers(1, &_bufferId);
        
        auto format = AL_FORMAT_MONO8;
        
        
        if(stereo) {
            format = bitDepth == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
        } else {
            format = bitDepth == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
        }
 
        alBufferData(_bufferId, format, buffer.data(), buffer.size(), _sampleRate);
    }
    
    AudioClip::~AudioClip() {
    
    }

    auto AudioClip::Buffer() const -> std::vector<float> {
        return _buffer;
    }

    auto AudioClip::BufferId() const -> ALuint {
        return _bufferId;
    }


    auto AudioClip::SampleRate() const -> uint32_t {
        return _sampleRate;
    }

    auto AudioClip::BitDepth() const -> uint32_t {
        return _bitDepth;
    }
    
    auto AudioClip::Channels() const -> uint8_t {
        return _numChannels;
    }
}
