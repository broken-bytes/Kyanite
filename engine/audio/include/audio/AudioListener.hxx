#pragma once

namespace audio {
    class AudioListener {
        public:
        AudioListener();
        ~AudioListener();

        auto SetPosition(float x, float y, float z) -> void;
        auto SetRotation(float x, float y, float z) -> void;
    };
}