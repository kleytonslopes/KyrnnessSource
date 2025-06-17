#pragma once
#ifndef	KYRNNESS_PHYSICS_SYSTEM_HPP
#define KYRNNESS_PHYSICS_SYSTEM_HPP

#include "Core/Core.hpp"
#include "Physics/PhysicsDebug.hpp"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
//#include <PxPhysicsVersion.h>
#include <cooking/PxConvexMeshDesc.h>
#include <cooking/PxTriangleMeshDesc.h>
#include <cooking/PxCooking.h>
#include <extensions/PxDefaultStreams.h>
#include <extensions/PxExtensionsAPI.h>
#include <geometry/PxHeightField.h>
#include <geometry/PxHeightFieldDesc.h>
#include <geometry/PxTriangleMesh.h>
//#include <geometry/PxTriangleMeshDesc.h>
#include <cooking/PxCooking.h>

class UApplication;
struct Vertex;

namespace CollisionGroup
{
	//constexpr uint32_t Default = (1 << 0);
	//constexpr uint32_t Player = (1 << 1);
	//constexpr uint32_t Enemy = (1 << 2);
	//constexpr uint32_t Environment = (1 << 3);

	constexpr physx::PxU32 Player = 1 << 0;
	constexpr physx::PxU32 Environment = 1 << 1;
}

namespace CollisionMask
{
	constexpr physx::PxU32 Player = CollisionGroup::Environment; 
	constexpr physx::PxU32 Environment = CollisionGroup::Player;

	//constexpr uint32_t Everything = 0xFFFFFFFF;
	//constexpr uint32_t Environment = CollisionGroup::Environment;
	//constexpr uint32_t PlayerOnly = CollisionGroup::Player;
}

class UPhysicsSystem
{
public:
	UPhysicsSystem(UApplication* application);
	virtual ~UPhysicsSystem() = default;

	UPhysicsSystem(const UPhysicsSystem&) = delete;
	UPhysicsSystem& operator=(const UPhysicsSystem&) = delete;
	UPhysicsSystem(UPhysicsSystem&&) = delete;
	UPhysicsSystem& operator=(UPhysicsSystem&&) = delete;

	void Initialize();
	void Update(float deltaTime);
	void Destroy();

	physx::PxScene* GetScene();

	physx::PxRigidStatic* CreateStaticBody(const glm::vec3& location, const glm::vec3& boundBox, const glm::vec3& rotationEuler);
	physx::PxRigidStatic* CreateTriangleMeshCollision(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices, const glm::vec3& location, const glm::vec3& rotationEuler, const glm::vec3& scale);
	physx::PxController* CreateCapsuleController(physx::PxCapsuleControllerDesc& desc);

private:
	UApplication* m_Application = nullptr;
	
	///float m_Gravity = -100.00f; // Default gravity value

	std::unique_ptr<UPhysicsDebug> m_PhysicsDebug;

	std::unordered_map<physx::PxController*, physx::PxController*> m_CharacterControllers;

	physx::PxDefaultAllocator m_DefaultAllocatorCallback;
	physx::PxDefaultErrorCallback m_DefaultErrorCallback;
	physx::PxTolerancesScale m_TolerancesScale;

	physx::PxFoundation* m_Foundation = NULL;
	physx::PxPhysics* m_Physics = NULL;
	physx::PxDefaultCpuDispatcher* m_Dispatcher = NULL;
	physx::PxScene* m_Scene = NULL;
	physx::PxMaterial* m_Material = NULL;
	physx::PxPvd* m_Pvd = NULL;
	physx::PxControllerManager* m_ControllerManager = NULL;
	//physx::PxCooking* m_Cooking = NULL;
};

#endif // KYRNNESS_PHYSICS_SYSTEM_HPP
