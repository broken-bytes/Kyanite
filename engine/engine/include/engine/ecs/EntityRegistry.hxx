#pragma once 

#include <rendering/IMeshRenderer.hxx>

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

namespace rendering = kyanite::engine::rendering;

namespace ecs::EntityRegistry {
	/**
	* @brief Initializes the entity registry
	*/
	auto Init(rendering::IMeshRenderer* renderer, bool debugServer) -> void;

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
	auto GetComponentBuffer(ecs_iter_t* iter, size_t componentSize, uint8_t index) -> void*;
}