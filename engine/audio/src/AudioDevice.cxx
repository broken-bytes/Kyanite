#include "audio/AudioDevice.hxx"

#include <al/alut.h>

namespace audio {
    AudioDevice::AudioDevice() {
        auto* device = alcOpenDevice(nullptr);
        
        if (!device) {
            // TODO: Throw error
        }
        
        auto* context = alcCreateContext(device, nullptr);
        
        if(!context) {
            // TODO: Throw error
        }
                
        _device = device;
        _context = context;
        alcMakeContextCurrent(context);        
        alcCaptureStart(_device);        
    }

    AudioDevice::~AudioDevice() {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(_context);
		alcCloseDevice(_device);
	}
}
