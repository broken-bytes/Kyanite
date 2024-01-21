#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#ifdef __cplusplus 
extern "C" {
#endif

	/**
	 * @brief Initialies the core module
	 *
	 */
	EXPORTED void Bridge_Core_Init();

	/**
	 * @brief Creates a new window
	 *
	 * @param title The title of the Window
	 * @param posX The position horizontal of the window
	 * @param posY The vertical position of the window
	 * @param width The width of the window
	 * @param height The height of the window
	 * @param flags Flas the window should use
	 * @param renderBackend The render backend to use for the window (0 = OpenGL, 1 = Vulkan, 2 = D3D12)
	 * @return uint64_t* Returns a pointer to the window
	 */
	EXPORTED NativePointer Bridge_Core_CreateWindow(
		const char* title,
		uint32_t* posX,
		uint32_t* posY,
		uint32_t width,
		uint32_t height,
		uint32_t flags,
		uint8_t renderBackend,
		bool silent
	);

	/**
	 * @brief Creates a new window
	 *
	 * @param title The native window
	 * @return uint64_t* Returns a pointer to the window
	 */
	EXPORTED NativePointer Bridge_Core_CreateWindowFromNative(NativePointer nativeWindow);

	/**
	 * @brief Destroys an audiosource via its pointer
	 *
	 * @param window The window
	 */
	EXPORTED void Bridge_Core_DestroyWindow(NativePointer window);

	/**
	 * @brief Shows the window
	 *
	 * @param window The window to be shown
	 */
	EXPORTED void Bridge_Core_ShowWindow(NativePointer window);

	/**
	 * @brief Hides the window
	 *
	 * @param window The window to be hidden
	 */
	EXPORTED void Bridge_Core_HideWindow(NativePointer window);

#ifdef __cplusplus 
}
#endif
