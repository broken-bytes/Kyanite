#pragma once


#include <shared/Exported.hxx>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif

	/**
	 * @brief Inits the input system
	 *
	 */
	EXPORTED void Bridge_Iput_Init();

	/**
	 * @brief Polls from the input system
	 *
	 */
	EXPORTED void Bridge_Input_Poll();

	/**
	 * @brief Gets the state of the given keyboard button
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED uint8_t Bridge_Input_GetKeyboardButtonState(uint32_t buttonId);

	/**
	 * @brief Gets the state of the given mouse button
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED uint8_t Bridge_Input_GetMouseButtonState(uint32_t buttonId);

	/**
	 * @brief Gets the value of the given mouse axis
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED float Bridge_Input_GetMouseAxis(uint8_t axis);

	/**
	 * @brief Gets the state of the given controller button
	 * @param uint8_t The controller to be used
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED uint8_t Bridge_Input_GetControllerButtonState(uint8_t controllerId, uint8_t buttonId);

	/**
	 * @brief Gets the value of the given controller axis
	 * @param uint8_t The controller to be used
	 * @param buttonId The Id of the button to query
	 *
	 */
	EXPORTED float Bridge_Input_GetControllerAxis(uint8_t controllerId, uint8_t axis);

#ifdef __cplusplus 
}
#endif
