#pragma once

#include <glm/glm.hpp>
#include <functional>

namespace PhysicsHandler {
	auto Init(std::function<void*(uint64_t)> transformQuery) -> void;
	auto Deinit() -> void;
	auto Update(float deltaTime) -> void;
	auto SetPosition(uint64_t entityId, glm::vec3 position);
	auto AddRigidbody(uint64_t entityId, void* transform, float mass, bool isStatic) -> void;
	auto DeleteRigidBody(uint64_t entityId) -> void;
	auto AddBoxCollider(
		uint64_t entityId,
		glm::vec3 bounds
	) -> void;
	auto AddSphereCollider(uint64_t entityId, float radius) -> void;
	auto AddCapsuleCollider(uint64_t entityId, float radius, float halfHeight) -> void;
}