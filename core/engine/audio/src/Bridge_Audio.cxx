#include "audio/Bridge_Audio.h"
#include "audio/Audio.hxx"
#include "audio/AudioDevice.hxx"
#include "audio/AudioSource.hxx"
#include "audio/AudioClip.hxx"

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

namespace audio = kyanite::engine::audio;

void Audio_Init() {
	// NOOP
}

NativePointer Audio_CreateDevice() {
    return reinterpret_cast<NativePointer>(audio::CreateDevice());
}

NativePointer Audio_CreateAudioSource(float x, float y, float z) {
    return reinterpret_cast<NativePointer>(new audio::AudioSource(x, y, z));
}

void Audio_DestroyDevice(NativePointer device) {
	delete reinterpret_cast<audio::AudioDevice*>(device);
}

void Audio_DestroyAudioSource(NativePointer source) {
    delete reinterpret_cast<audio::AudioSource*>(source);
}

NativePointer Audio_CreateAudioClip(const char* path) {
    return reinterpret_cast<NativePointer>(audio::CreateAudioClip(path));
}

uint32_t Audio_ClipGetSampleRate(NativePointer clip) {
    auto audioClip = reinterpret_cast<audio::AudioClip*>(clip);
    return audioClip->SampleRate();
}

uint32_t Audio_ClipGetBitDepth(NativePointer clip) {
    auto audioClip = reinterpret_cast<audio::AudioClip*>(clip);
    return audioClip->BitDepth();
}

uint8_t Audio_ClipGetChannels(NativePointer clip) {
    auto audioClip = reinterpret_cast<audio::AudioClip*>(clip);
    return audioClip->Channels();
}

void Audio_AudioSourceSetAudioClip(NativePointer source, NativePointer clip) {
    auto sourceP = reinterpret_cast<audio::AudioSource*>(source);
    auto clipP = reinterpret_cast<audio::AudioClip*>(clip);
    
    sourceP->SetClip(clipP);
}

void Audio_AudioSourceStart(NativePointer source) {
    reinterpret_cast<audio::AudioSource*>(source)->Start();
}

void Audio_AudioSourceStop(NativePointer source) {
    reinterpret_cast<audio::AudioSource*>(source)->Stop();
}

void Audio_AudioSourceSetLooping(NativePointer source, bool looping) {
    reinterpret_cast<audio::AudioSource*>(source)->SetLooping(looping);
}
