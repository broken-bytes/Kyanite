#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>

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
	* @brief Sets the parent of an entity
	* @param entity The entity to set the parent of
	* @param parent The parent to set
	*/
	EXPORTED void ECS_SetParent(uint64_t entity, uint64_t parent);

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
	* @brief Gets a component from an entity
	* @param entity The entity to get the component from
	* @param component The component to get
	* @return The component
	*/
	EXPORTED NativePointer ECS_GetComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Gets a component from an entity
	* @param entity The entity to get the component from
	* @param len The length of the components
	* @return Pointer to the list of componentIds
	* @note Caller is responsible for freeing the memory
	*/
	EXPORTED uint64_t* ECS_GetAllComponents(uint64_t entity, size_t* len);

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

	/**
	* Get entity by name
	* @param name The name of the entity
	* @param entity The entity if found, otherwise null
	*/
	EXPORTED uint64_t ECS_GetEntityByName(const char* name);

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the name of
	* @return The name of the entity
	*/
	EXPORTED const char* ECS_GetEntityName(uint64_t entity);

	/**
	* @brief Gets the parent of an entity
	* @param entity The entity to get the parent of
	* @return The parent of the entity or 0 if it has no parent
	*/
	EXPORTED uint64_t ECS_GetParent(uint64_t entity);

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the childs of
	* @param callback The callback to call for each child
	*/
	EXPORTED void ECS_ForEachChild(uint64_t parent, void (*callback)(const char* name, uint64_t id));
#ifdef __cplusplus 
}
#endif