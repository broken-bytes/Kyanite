#include "audio/Audio.hxx"
#include "audio/Bridge_Audio.h"
#include "audio/AudioDevice.hxx"

#include <AudioFile.h>
#include <al/alut.h>

#include <string>
#include <vector>


namespace kyanite::engine::audio {
    std::vector<AudioClip*> AudioClips = {};

    auto QueryDevices() -> std::vector<std::string> {
        return {};
    }

    auto CreateDevice() -> AudioDevice* {
        return new AudioDevice();
    }

    auto CreateAudioClip(std::string path) -> AudioClip* {
        AudioFile<float> data;
        data.shouldLogErrorsToConsole(false);
        if(!data.load(path)) {
            //throw FileNotFoundError(path);
        }
        
        ALenum format;
        float* buffer;
        ALsizei size;
        ALsizei freq;
        
        alutLoadWAVFile(
            const_cast<ALbyte*>(reinterpret_cast<const ALbyte*>(path.c_str())),
            &format,
            reinterpret_cast<void**>(&buffer),
            &size,
            &freq,
            0
        );
        
        std::vector<float> buffV(size);
        for(int x = 0; x < size; x++) {
            buffV[x] = buffer[x];
        }

        auto clip = new AudioClip(
            buffV,
            data.getSampleRate(),
            data.getBitDepth(),
            data.getNumSamplesPerChannel(),
            data.getNumChannels(),
            data.getLengthInSeconds(),
            data.isStereo()
        );
        
        AudioClips.push_back(clip);
        
        return clip;
    }

    auto LoadAudioClip(std::string_view path) -> AudioClip {
        //auto buffer = core::LoadFileToBuffer(path);

        return AudioClip({}, 0, 0, 0, 0, 0 , true);
    }
}
