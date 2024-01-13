#pragma once

#include <core/Exported.hxx>
#include <core/NativePointer.hxx>

#include <stdint.h>

#ifdef __cplusplus 
extern "C" {
#endif

	/**
	 * @brief Initializes the ECS
	 */
	EXPORTED void Bridge_ECS_Init(NativePointer meshRenderer);

	/**
	 * @brief Creates a new entity
	 *
	 * @return The new entity
	 */
	EXPORTED uint64_t Bridge_ECS_CreateEntity();

	/**
	 * @brief Destroys an entity
	 *
	 * @param entity The entity to destroy
	 */
	EXPORTED void Bridge_ECS_DestroyEntity(uint64_t entity);

	/**
	* @brief Adds a component to an entity
	* @param entity The entity to add the component to
	* @param component The component to add
	*/
    EXPORTED void Bridge_ECS_AddComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Removes a component from an entity
	* @param entity The entity to remove the component from
	* @param component The component to remove
	*/
	EXPORTED void Bridge_ECS_RemoveComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Registers a component type
	* @param size The size of the component type
	* @return The component type
	*/
	EXPORTED uint64_t Bridge_ECS_RegisterComponent(size_t size);

#ifdef __cplusplus 
}
#endif
