#pragma once 

#include <rendering/IMeshRenderer.hxx>

#define FLECS_SYSTEM
#define FLECS_META_C
#define FLECS_PIPELINE
#define FLECS_TIMER
#define FLECS_REST
#define FLECS_STATS
#include <flecs.h>

#include <stdint.h>

namespace ecs::EntityRegistry {
	/**
	* @brief Initializes the entity registry
	*/
	auto Init(rendering::IMeshRenderer* renderer) -> void;

	/**
	* @brief Gets the entity registry
	* @return The entity registry
	*/
	auto GetRegistry() -> ecs_world_t*;

	/**
	* @brief Creates an entity
	* @return The created entity
	*/
	auto CreateEntity() -> ecs_entity_t;

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
	auto CreateComponent(size_t size, size_t alignment) -> ecs_entity_t;

	/**
	* @brief Adds a component to an entity
	* @param entity The entity to add the component to
	* @param type The type of the component to add
	*/
	auto AddComponent(ecs_entity_t entity, ecs_entity_t type) -> void;

	/**
	* @brief Removes a component from an entity
	* @param entity The entity to remove the component from
	*/
	auto RemoveComponent(ecs_entity_t entity, ecs_entity_t component) -> void;
}