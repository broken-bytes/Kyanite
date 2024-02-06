#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif
	/**
	 * @brief Initializes the ECS
	 */
	EXPORTED void ECS_Init(NativePointer logger, bool isDebug);

	/**
	* @brief Updates the engine
	* @param delta The time since the last frame
	*/
	EXPORTED void ECS_Update(float delta);

	/**
	 * @brief Creates a new entity
	 *
	 * @return The new entity
	 */
	EXPORTED uint64_t ECS_CreateEntity(const char* name);

	/**
	 * @brief Destroys an entity
	 *
	 * @param entity The entity to destroy
	 */
	EXPORTED void ECS_DestroyEntity(uint64_t entity);

	/**
	* @brief Adds a component to an entity
	* @param entity The entity to add the component to
	* @param component The component to add
	*/
	EXPORTED void ECS_AddComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Sets a component of an entity
	* @param entity The entity to use
	* @param component The component to set
	* @param data The data to set the component to
	*/
	EXPORTED void ECS_SetComponent(uint64_t entity, uint64_t component, void* data);

	/**
	* @brief Removes a component from an entity
	* @param entity The entity to remove the component from
	* @param component The component to remove
	*/
	EXPORTED void ECS_RemoveComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Registers a component type
	* @param size The size of the component type
	* @param alignment The alignment of the component type
	* @return The component type
	*/
	EXPORTED uint64_t ECS_RegisterComponent(const char* name, size_t size, size_t alignment);

	/**
	* @brief Registers a system
	* @param funcPtr The function pointer to the system. Takes the components and the number of components as arguments
	*/
	EXPORTED uint64_t ECS_RegisterSystem(const char* name, uint64_t* filter, size_t filterLen, void (*func)(NativePointer));

	/**
	* @brief Gets components for an iterator
	* @param iterator The iterator to get the components from
	* @param index The index of the component
	* @param componentSize The size of the component
	* @return The components
	*/
	EXPORTED NativePointer ECS_GetComponentsFromIterator(NativePointer iterator, uint8_t index, size_t componentSize);

	/**
	* @brief Gets the number of components for an iterator
	* @param iterator The iterator to get the components from
	* @return The number of components
	*/
	EXPORTED size_t ECS_GetIteratorSize(NativePointer iterator);
#ifdef __cplusplus 
}
#endif