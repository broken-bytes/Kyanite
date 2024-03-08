#pragma once

struct ALCdevice;
struct ALCcontext;

namespace kyanite::engine::audio {
    class AudioDevice {
        public:
        AudioDevice();
        ~AudioDevice();
        
        private:
        ALCdevice* _device;
        ALCcontext* _context;
    };
}
