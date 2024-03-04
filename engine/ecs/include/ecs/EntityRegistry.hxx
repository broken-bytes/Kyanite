#pragma once 

#define FLECS_CORE
#define FLECS_SYSTEM
#define FLECS_PIPELINE
#define FLECS_META
#define FLECS_META_C
#define FLECS_PIPELINE
#define FLECS_HTTP
#define FLECS_TIMER
#define FLECS_REST
#define FLECS_STATS
#define FLECS_MONITOR
#define FLECS_CPP
#include <flecs.h>

#include <stdint.h>
#include <string>
#include <vector>

namespace ecs::EntityRegistry {
	/**
	* @brief Initializes the entity registry
	*/
	auto Init(bool debugServer) -> void;

	/**
	* @brief Gets the entity registry
	* @return The entity registry
	*/
	auto GetRegistry() -> ecs_world_t*;

	/**
	* @brief Creates an entity
	* @return The created entity
	*/
	auto CreateEntity(std::string name) -> ecs_entity_t;

	/**
	* @brief Sets the parent of an entity
	* @param entity The entity to set the parent of
	* @param parent The parent to set
	*/
	auto SetParent(ecs_entity_t entity, ecs_entity_t parent) -> void;

	/**
	* @brief Destroys an entity
	* @param entity The entity to destroy
	*/
	auto DestroyEntity(ecs_entity_t entity) -> void;

	/**
	* @brief Creates a component
	* @param size The size of the component
	* @param alignment The alignment of the component
	* @return The created component
	*/
	auto CreateComponent(std::string name, size_t size, size_t alignment) -> ecs_entity_t;

	/**
	* @brief Adds a component to an entity
	* @param entity The entity to add the component to
	* @param type The type of the component to add
	*/
	auto AddComponent(ecs_entity_t entity, ecs_entity_t type) -> void;

	/**
	* @brief Sets a component on an entity
	* @param entity The entity to set the component on
	* @param component The component to set
	* @param data The data to set the component to
	*/
	auto SetComponent(ecs_entity_t entity, ecs_entity_t component, void* data) -> void;

	/**
	* @brief Removes a component from an entity
	* @param entity The entity to remove the component from
	*/
	auto RemoveComponent(ecs_entity_t entity, ecs_entity_t component) -> void;

	/**
	* @brief Gets a component from an entity
	* @param entity The entity to get the component from
	* @param component The component to get
	* @return The component
	*/
	auto GetComponent(ecs_entity_t entity, ecs_entity_t component) -> const void*;

	/**
	* @brief Gets all the components of an entity
	* @param entity The entity to get the components of
	* @return The components
	* @note This function is slow and should be used sparingly
	*/
	auto GetEntityComponents(ecs_entity_t entity) -> std::vector<ecs_entity_t>;

	/**
	* @brief Advances the ecs world by delta time
	* @param delta The delta time
	*/
	auto Update(float delta) -> void;

	/**
	* @brief Registers a system
	* @param func The function to register
	*/
	auto RegisterSystem(std::string name, std::vector<ecs_entity_t> filter, void (*func)(ecs_iter_t* it)) -> ecs_entity_t;

	/**
	* @brief Gets all the components of an entity for a given index
	* @param iter The iterator
	* @param componentSize The size of the component
	* @param index The index of the component
	* @return The components
	*/
	auto GetComponentBuffer(ecs_iter_t* iter, uint8_t index, size_t componentSize) -> void*;

	/**
	* @brief Gets the entity by name
	* @param name The name of the entity
	* @return The entity
	*/
	auto GetEntityByName(std::string_view name) -> uint64_t;

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the name of
	* @return The name
	*/
	auto GetEntityName(ecs_entity_t entity) -> const char*;

	/**
	* @brief Gets the parent of an entity
	* @param entity The entity to get the parent of
	* @return The parent
	* @note Returns null if the entity has no parent
	*/
	auto GetParent(ecs_entity_t entity) -> ecs_entity_t;

	/**
	* @brief Gets the name of an entity
	* @param entity The entity to get the childs of
	* @param callback The callback to call for each child
	*/
	auto ForEachChild(ecs_entity_t parent, void (*callback)(const char* name, ecs_entity_t id)) -> void;
}