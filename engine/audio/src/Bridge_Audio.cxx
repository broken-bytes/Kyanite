#include "audio/Bridge_Audio.h"
#include "audio/Audio.hxx"
#include "audio/AudioDevice.hxx"
#include "audio/AudioSource.hxx"
#include "audio/AudioClip.hxx"

#include <core/Exported.hxx>
#include <core/NativePointer.hxx>

NativePointer Bridge_Audio_CreateDevice() {
    return reinterpret_cast<NativePointer>(audio::CreateDevice());
}

NativePointer Bridge_Audio_CreateAudioSource(float x, float y, float z) {
    return reinterpret_cast<NativePointer>(new audio::AudioSource(x, y, z));
}

void Bridge_Audio_DestroyDevice(NativePointer device) {
	delete reinterpret_cast<audio::AudioDevice*>(device);
}

void Bridge_Audio_DestroyAudioSource(NativePointer source) {
    delete reinterpret_cast<audio::AudioSource*>(source);
}

NativePointer Bridge_Audio_CreateAudioClip(const char* path) {
    return reinterpret_cast<NativePointer>(audio::CreateAudioClip(path));
}

uint32_t Bridge_Audio_ClipGetSampleRate(NativePointer clip) {
    auto audioClip = reinterpret_cast<audio::AudioClip*>(clip);
    return audioClip->SampleRate();
}

uint32_t Bridge_Audio_ClipGetBitDepth(NativePointer clip) {
    auto audioClip = reinterpret_cast<audio::AudioClip*>(clip);
    return audioClip->BitDepth();
}

uint8_t Bridge_Audio_ClipGetChannels(NativePointer clip) {
    auto audioClip = reinterpret_cast<audio::AudioClip*>(clip);
    return audioClip->Channels();
}

void Bridge_Audio_AudioSourceSetAudioClip(NativePointer source, NativePointer clip) {
    auto sourceP = reinterpret_cast<audio::AudioSource*>(source);
    auto clipP = reinterpret_cast<audio::AudioClip*>(clip);
    
    sourceP->SetClip(clipP);
}

void Bridge_Audio_AudioSourceStart(NativePointer source) {
    reinterpret_cast<audio::AudioSource*>(source)->Start();
}

void Bridge_Audio_AudioSourceStop(NativePointer source) {
    reinterpret_cast<audio::AudioSource*>(source)->Stop();
}

void Bridge_Audio_AudioSourceSetLooping(NativePointer source, bool looping) {
    reinterpret_cast<audio::AudioSource*>(source)->SetLooping(looping);
}
