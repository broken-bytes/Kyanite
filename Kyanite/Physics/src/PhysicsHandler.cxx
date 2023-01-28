#include "Physics/PhysicsHandler.hxx"
#include <map>
#include <memory>
#include "PhysX/PxPhysicsAPI.h"
#include "glm/glm.hpp"
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace PhysicsHandler {
	class Allocator : public physx::PxAllocatorCallback {
	public:
		Allocator() {

		}

		virtual ~Allocator()
		{

		}

		virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) {
			//std::cout << "Creating " << size << "bytes of memory for " << typeName << "at " << filename << ":" << line << std::endl;
			return std::malloc(size);
		}

		virtual void deallocate(void* ptr) {
			delete ptr;
		}
	};

	class Error : public physx::PxErrorCallback {
	public:
		Error() {

		}

		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
		{
			std::cerr << code << " " << message << " in " << file << ":" << line << std::endl;
		}
	};

	class CpuDispatcher : public physx::PxCpuDispatcher
	{
	public:
		CpuDispatcher(physx::PxU32 numThreads)
		{
			// Create the specified number of worker threads
			for (int x = 0; x < numThreads; x++)
			{
				_workerThreads.emplace_back([this]()
					{
						while (_running)
						{
							physx::PxBaseTask* task = getNextTask();
							if (task)
							{
								task->run();
								task->release();
							}
						}
					});
			}
		}

		virtual void submitTask(physx::PxBaseTask& task)
		{
			std::scoped_lock lock{ _mutex };
			// Add the task to the task queue
			_taskQueue.push(&task);
		}

		virtual physx::PxU32 getWorkerCount() const
		{
			// Return the number of worker threads
			return static_cast<physx::PxU32>(_workerThreads.size());
		}

		~CpuDispatcher()
		{
			_running = false;
			for (auto& thread : _workerThreads)
			{
				thread.join();
			}
		}

	private:
		physx::PxBaseTask* getNextTask()
		{
			std::scoped_lock lock{ _mutex };
			if (!_taskQueue.empty())
			{
				auto task = _taskQueue.front();
				_taskQueue.pop();
				return task;
			}
			return nullptr;
		}

		std::vector<std::thread> _workerThreads;
		std::atomic<bool> _running{ true };
		std::queue<physx::PxBaseTask*> _taskQueue;
		std::mutex _mutex;
	};

	Error* ErrorCallback;
	Allocator* AllocatorCallback;
	CpuDispatcher* Dispatcher;
	physx::PxFoundation* Foundation;
	physx::PxPhysics* Physics;
	physx::PxScene* Scene;
	float Timestep = 1 / 55.0f;
	float CurrentStep = 0;

	physx::PxMaterial* DefaultMaterial;

	std::map<uint64_t, physx::PxShape*> Shapes;
	std::map<uint64_t, physx::PxRigidDynamic*> DynamicBodies;
	std::map<uint64_t, physx::PxRigidStatic*> StaticBodies;
	std::function<void* (uint64_t)> TransformQuery;


	auto AttachToRigidIfExists(uint64_t entityId, physx::PxShape* shape) -> void {
		auto staticIter = StaticBodies.find(entityId);
		if (staticIter != StaticBodies.end()) {
			staticIter->second->attachShape(*shape);

			return;
		}
		auto dynamicIter = DynamicBodies.find(entityId);
		if (dynamicIter != DynamicBodies.end()) {
			dynamicIter->second->attachShape(*shape);
			dynamicIter->second->wakeUp();
			return;
		}
	}

	physx::PxFilterFlags DefaultFilterShader(
		physx::PxFilterObjectAttributes attributes0,
		physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1,
		physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock,
		physx::PxU32 constantBlockSize
	) {
		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	auto Init(std::function<void* (uint64_t)> transformQuery) -> void {
		AllocatorCallback = new Allocator();
		ErrorCallback = new Error();
		Dispatcher = new CpuDispatcher(8);
		Foundation = PxCreateFoundation(
			PX_PHYSICS_VERSION,
			*AllocatorCallback,
			*ErrorCallback
		);

		auto scale = physx::PxTolerancesScale();
		Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, scale);
		DefaultMaterial = Physics->createMaterial(0.5f, 0.5f, 0.1f);
		physx::PxSceneDesc sceneDesc(Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = Dispatcher;
		sceneDesc.filterShader = DefaultFilterShader;
		Scene = Physics->createScene(sceneDesc);
		Scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
		TransformQuery = transformQuery;
	}

	auto Deinit() -> void {
		DefaultMaterial->release();
		Scene->release();
		Physics->release();
		Foundation->release();
	}

	auto Update(float deltaTime) -> void {
		CurrentStep += deltaTime;

		if (CurrentStep < Timestep) {
			return;
		}

		Scene->simulate(Timestep);
		Scene->fetchResults(true);

		for (auto& body : StaticBodies) {
			auto pose = body.second->getGlobalPose();
			auto entityId = (uint64_t)(body.second->userData);

			auto transform = (glm::vec3*)TransformQuery(entityId);
			transform->x = pose.p.x;
			transform->y = pose.p.y;
			transform->z = pose.p.z;
			auto rotation = (glm::vec4*)((glm::vec3*)TransformQuery(entityId) + 1);
			rotation->w = pose.q.w;
			rotation->x = pose.q.x;
			rotation->y = pose.q.y;
			rotation->z = pose.q.z;
		}

		for (auto& body : DynamicBodies) {
			auto pose = body.second->getGlobalPose();
			auto entityId = (uint64_t)(body.second->userData);

			auto transform = (glm::vec3*)TransformQuery(entityId);
			transform->x = pose.p.x;
			transform->y = pose.p.y;
			transform->z = pose.p.z;
			auto rotation = (glm::vec4*)((glm::vec3*)TransformQuery(entityId) + 1);
			rotation->w = pose.q.w;
			rotation->x = pose.q.x;
			rotation->y = pose.q.y;
			rotation->z = pose.q.z;
		}

		CurrentStep = 0;
	}

	auto SetPosition(uint64_t entityId, glm::vec3 position) {

	}

	auto AddRigidbody(uint64_t entityId, void* transform, float mass, bool isStatic) -> void {
		physx::PxRigidActor* actor = nullptr;
		glm::vec3 origin = *reinterpret_cast<glm::vec3*>(transform);

		if (isStatic) {
			StaticBodies.insert({ entityId, Physics->createRigidStatic({ origin.x, origin.y, origin.z }) });
			actor = StaticBodies[entityId];
		}
		else {
			auto dynamic = Physics->createRigidDynamic({ origin.x, origin.y, origin.z });
			DynamicBodies.insert({ entityId, dynamic });
			actor = DynamicBodies[entityId];
			dynamic->wakeUp();
		}

		if (Shapes.contains(entityId)) {
			actor->attachShape(*Shapes[entityId]);
		}

		actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
		Scene->addActor(*actor);

		actor->userData = (void*)entityId;
	}

	auto DeleteRigidBody(uint64_t entityId) -> void {

	}

	auto AddBoxCollider(
		uint64_t entityId,
		glm::vec3 bounds
	) -> void {
		physx::PxVec3 dimensions(bounds.x / 2, bounds.y / 2, bounds.z / 2);
		auto shape = Physics->createShape(physx::PxBoxGeometry(dimensions), *DefaultMaterial);
		Shapes.insert({ entityId, shape });
		AttachToRigidIfExists(entityId, shape);
	}

	auto AddSphereCollider(uint64_t entityId, float radius) -> void {
		physx::PxSphereGeometry sphere(radius);
		auto shape = Physics->createShape(sphere, *DefaultMaterial);
		Shapes.insert({ entityId, shape });
		AttachToRigidIfExists(entityId, shape);
	}

	auto AddCapsuleCollider(uint64_t entityId, float radius, float halfHeight) -> void {
		physx::PxCapsuleGeometry capsule(radius, halfHeight);
		auto shape = Physics->createShape(capsule, *DefaultMaterial);
		Shapes.insert({ entityId, shape });
		AttachToRigidIfExists(entityId, shape);
	}
}