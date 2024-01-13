#pragma once

#include <stdint.h>
#include <vector>

namespace audio {
    class AudioClip {
        public:
        AudioClip(
            std::vector<float> buffer,
            int sampleRate, 
            int bitDepth, 
            int numSamples, 
            int numChannels, 
            float length, 
            bool stereo
        );
        ~AudioClip();

        auto Buffer() const -> std::vector<float>;
        auto BufferId() const -> uint32_t;
        auto SampleRate() const -> uint32_t;
        auto BitDepth() const -> uint32_t;
        auto Channels() const -> uint8_t;

        private:
        std::vector<float> _buffer;
        uint32_t _bufferId;
        int _sampleRate;
        int _bitDepth;
        int _numSamples; 
        int _numChannels;
        float _length;
        bool _stereo;
    };
}
