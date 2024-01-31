#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>

#ifdef __cplusplus 
extern "C" {
#endif

	enum Mode {
		EDITOR,
		GAME_DEBUG,
		GAME_RELEASE
	};

	/**
	 * @brief Initializes the ECS
	 */
	EXPORTED void Bridge_Engine_Init(NativePointer window, NativePointer assetLoader, enum Mode mode, NativePointer logger);

	/**
	* @brief Updates the engine
	* @param delta The time since the last frame
	*/
	EXPORTED void Bridge_Engine_Update(float delta);

	/**
	 * @brief Creates a new entity
	 *
	 * @return The new entity
	 */
	EXPORTED uint64_t Bridge_Engine_CreateEntity(const char* name);

	/**
	 * @brief Destroys an entity
	 *
	 * @param entity The entity to destroy
	 */
	EXPORTED void Bridge_Engine_DestroyEntity(uint64_t entity);

	/**
	* @brief Adds a component to an entity
	* @param entity The entity to add the component to
	* @param component The component to add
	*/
	EXPORTED void Bridge_Engine_AddComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Sets a component of an entity
	* @param entity The entity to use
	* @param component The component to set
	* @param data The data to set the component to
	*/
	EXPORTED void Bridge_Engine_SetComponent(uint64_t entity, uint64_t component, void* data);

	/**
	* @brief Removes a component from an entity
	* @param entity The entity to remove the component from
	* @param component The component to remove
	*/
	EXPORTED void Bridge_Engine_RemoveComponent(uint64_t entity, uint64_t component);

	/**
	* @brief Registers a component type
	* @param size The size of the component type
	* @param alignment The alignment of the component type
	* @return The component type
	*/
	EXPORTED uint64_t Bridge_Engine_RegisterComponent(const char* name, size_t size, size_t alignment);

	/**
	* @brief Registers a system
	* @param funcPtr The function pointer to the system. Takes the components and the number of components as arguments
	*/
	EXPORTED uint64_t Bridge_Engine_RegisterSystem(const char* name, uint64_t* filter, size_t filterLen, void (*func)(NativePointer));

	/**
	* @brief Gets components for an iterator
	* @param iterator The iterator to get the components from
	* @param index The index of the component
	* @param componentSize The size of the component
	* @return The components
	*/
	EXPORTED NativePointer Bridge_Engine_GetComponentsFromIterator(NativePointer iterator, uint8_t index, size_t componentSize);

	/**
	* @brief Gets the number of components for an iterator
	* @param iterator The iterator to get the components from
	* @return The number of components
	*/
	EXPORTED size_t Bridge_Engine_GetIteratorSize(NativePointer iterator);

	// --- Asset loading ---
	/**
	* @brief Loads an asset package
	* @param path The path to the asset package
	* @return The asset package
	*/
	EXPORTED NativePointer Bridge_Engine_LoadAssetPackages(const char* path, size_t* numPackages);

	/**
	* @brief Loads a texture
	* @param uuid The uuid of the texture
	* @return The texture
	*/
	EXPORTED NativePointer Bridge_Engine_LoadTexture(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Loads a mesh
	* @param uuid The uuid of the mesh
	* @return The mesh
	*/
	EXPORTED NativePointer Bridge_Engine_LoadMesh(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Loads a material
	* @param uuid The uuid of the material
	* @return The material
	*/
	EXPORTED NativePointer Bridge_Engine_LoadMaterial(NativePointer assetPackage, const char* uuid);

	/**
	*	@brief Loads a shader
	* @param uuid The uuid of the shader
	* @return The shader
	*/
	EXPORTED NativePointer Bridge_Engine_LoadShader(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Loads an audioclip
	* @param uuid The uuid of the audioclip
	* @return The audioclip
	*/
	EXPORTED NativePointer Bridge_Engine_LoadAudioClip(NativePointer assetPackage, const char* uuid);

	/**
	* @brief Clears an asset
	* @param asset The asset to clear
	*/
	EXPORTED void Bridge_Engine_DisposeAsset(NativePointer asset);

#ifdef __cplusplus 
}
#endif
