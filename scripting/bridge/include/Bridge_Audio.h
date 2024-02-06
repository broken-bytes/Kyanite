#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif

	EXPORTED void Audio_Init();

	/**
	 * @brief Creates an audio device
	 *
	 * @return NativePointer
	 */
	EXPORTED NativePointer Audio_CreateDevice();

	/**
	 * @brief Destroys an audio device
	 *
	 * @param device The device to be destroyed
	 */
	EXPORTED void Audio_DestroyDevice(NativePointer device);

	/**
	 * @brief Creates a new audiosource.
	 *
	 * @param x X position of the source
	 * @param y Y position of the source
	 * @param z Z position of the source
	 * @return uint64_t The id of the source
	 */
	EXPORTED NativePointer Audio_CreateAudioSource(float x, float y, float z);

	/**
	 * @brief Destroys an audiosource via its id
	 *
	 * @param source The id of the source
	 */
	EXPORTED void Audio_DestroyAudioSource(NativePointer source);

	/**
	 * @brief Creates a new audioclip from a given file
	 *
	 * @param path The path of the file
	 * @return uint64_t The id of the clip
	 */
	EXPORTED NativePointer Audio_CreateAudioClip(const char* path);

	/**
	 * @brief Gets the sample rate of a given clip
	 *
	 * @param clip The clip to query
	 * @return uint32_t
	 */
	EXPORTED uint32_t Audio_ClipGetSampleRate(NativePointer clip);

	/**
	 * @brief Gets he bitdepth for a given clip
	 *
	 * @param clip The clip to be queried
	 * @return uint32_t
	 */
	EXPORTED uint32_t Audio_ClipGetBitDepth(NativePointer clip);

	/**
	 * @brief Gets the number of channels for a given clip
	 *
	 * @param clip The clip to be queried
	 * @return uint8_t
	 */
	EXPORTED uint8_t Audio_ClipGetChannels(NativePointer clip);

	/**
	 * @brief Plays an audio clip via an audiosource
	 *
	 * @param source The source to be used
	 * @param clip The clip to be played
	 */
	EXPORTED void Audio_AudioSourceSetAudioClip(NativePointer source, NativePointer clip);

	/**
	 * @brief Starts playback of an audiosource
	 *
	 * @param source The source
	 */
	EXPORTED void Audio_AudioSourceStart(NativePointer source);

	/**
	 * @brief Stops playback of an audiosource
	 *
	 * @param source The source
	 */
	EXPORTED void Audio_AudioSourceStop(NativePointer source);

	/**
	 * @brief Toggles looping for the given source
	 *
	 * @param source The source
	 */
	EXPORTED void Audio_AudioSourceSetLooping(NativePointer source, bool looping);


#ifdef __cplusplus 
}
#endif
