#include "audio/Bridge_Audio.hxx"
#include <GTest/GTest.h>
#include <iostream>

TEST(AudioBridge, TestAudioLoadAudioClip) {
    try {
        auto clip = Bridge_Audio_CreateAudioClip("./resources/buxtehude.wav");
    } catch(std::exception error) {
        FAIL();
    }
}

TEST(AudioBridge, TestAudioAudioClipGetSampleRate) {
    try {
        auto clip = Bridge_Audio_CreateAudioClip("./resources/buxtehude.wav");
        auto rate = Bridge_Audio_ClipGetSampleRate(clip);
        auto bitDepth = Bridge_Audio_ClipGetBitDepth(clip);
        auto channels = Bridge_Audio_ClipGetChannels(clip);
        
        // The dummy clip is known to be 11025
        if(rate != 11025) {
            FAIL();
        }
        
        // The dummy clip is known to be 8 bits deep
        if(bitDepth != 8) {
            FAIL();
        }
        
        // The dummy clip is known to be 1 channel
        if(channels != 1) {
            FAIL();
        }
    } catch(std::exception error) {
        FAIL();
    }
}
