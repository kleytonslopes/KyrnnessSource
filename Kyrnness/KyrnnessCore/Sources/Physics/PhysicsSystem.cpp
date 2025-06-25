#include "CorePCH.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Runtime/Application.hpp"
#include "Core/Vertex.hpp"
#include "GameFramework/Scene.hpp"


UPhysicsSystem::UPhysicsSystem(UApplication* application) : m_Application(application)
{
}

void UPhysicsSystem::Initialize()
{
	if (m_Application)
	{
		m_Application->OnUpdateEvent.AddListener(this, &UPhysicsSystem::Update);
	}

	m_PhysicsDebug = std::make_unique<UPhysicsDebug>();

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
	if (!m_Foundation)
		ThrowRuntimeError("PxFoundation creation Failed!");

	m_Pvd = PxCreatePvd(*m_Foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	m_TolerancesScale.length = 100;
	m_TolerancesScale.speed = 981;

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_TolerancesScale, true, m_Pvd);
	physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.f, -10.f, 0.f);
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);// 2 Thread
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// Create Simulation
	m_Material = m_Physics->createMaterial(0.0f, 0.0f, 0.0f);
	m_ControllerManager = PxCreateControllerManager(*m_Scene);
	
}

void UPhysicsSystem::Update(float deltaTime)
{
	// 1. Mover todos os controllers manualmente
	//for (auto& [controllerPtr, controller] : m_CharacterControllers)
	//{
	//	physx::PxVec3 gravityMotion(0.f, m_Application->GetScene()->GetGravity() * deltaTime, 0.f);
	//	controllerPtr->move(gravityMotion, 0.001f, deltaTime, physx::PxControllerFilters());
	//}

	m_Scene->simulate(deltaTime);
	m_Scene->fetchResults(true);
}

void UPhysicsSystem::Destroy()
{
	m_ControllerManager->release();
	m_Physics->release();

	m_Foundation->release();
}

physx::PxScene* UPhysicsSystem::GetScene()
{
	return m_Scene;
}

physx::PxRigidStatic* UPhysicsSystem::CreateStaticBody(const glm::vec3& location, const glm::vec3& boundBox, const glm::vec3& rotationEuler)
{
	physx::PxVec3 pxLocation(location.x, location.y, location.z);
	glm::quat glmQuat = glm::quat(glm::radians(rotationEuler));
	physx::PxQuat pxRotation(glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w);

	physx::PxTransform transform(pxLocation, pxRotation);

	physx::PxBoxGeometry boxGeom(
		physx::PxReal(boundBox.x * 0.5f),
		physx::PxReal(boundBox.y * 0.5f),
		physx::PxReal(boundBox.z * 0.5f)
	);

	physx::PxRigidStatic* body = physx::PxCreateStatic(*m_Physics, transform, boxGeom, *m_Material);

	m_Scene->addActor(*body);

	return body;

}

physx::PxRigidStatic* UPhysicsSystem::CreateTriangleMeshCollision(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices, const glm::vec3& location, const glm::vec3& rotationEuler, const glm::vec3& scale)
{
	physx::PxTolerancesScale tolerancesScale;
	physx::PxCookingParams cookingParams(tolerancesScale);

	cookingParams.meshPreprocessParams = physx::PxMeshPreprocessingFlag::eWELD_VERTICES;

	physx::PxTriangleMeshDesc meshDesc{};
	meshDesc.points.count = static_cast<uint32_t>(vertices.size());
	meshDesc.points.stride = sizeof(glm::vec3);
	meshDesc.points.data = vertices.data();

	meshDesc.triangles.count = static_cast<uint32_t>(indices.size() / 3);
	meshDesc.triangles.stride = sizeof(uint32_t) * 3;
	meshDesc.triangles.data = indices.data();

	physx::PxDefaultMemoryOutputStream cookedMeshOutput;

	if (!PxCookTriangleMesh(cookingParams, meshDesc, cookedMeshOutput))
	{
		throw std::runtime_error("Falha ao cozinhar a mesh do terreno.");
	}

	physx::PxDefaultMemoryInputData cookedMeshInput(cookedMeshOutput.getData(), cookedMeshOutput.getSize());
	physx::PxTriangleMesh* triangleMesh = m_Physics->createTriangleMesh(cookedMeshInput);

	physx::PxMaterial* material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

	glm::quat glmQuat = glm::quat(glm::radians(rotationEuler));
	physx::PxQuat pxQuat(glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w);

	physx::PxVec3 pxLocation(location.x, location.y, location.z);
	physx::PxTransform transform(pxLocation, pxQuat);

	physx::PxTriangleMeshGeometry geom(triangleMesh, physx::PxMeshScale(physx::PxVec3(scale.x, scale.y, scale.z), physx::PxQuat(physx::PxIdentity)));

	physx::PxRigidStatic* ground = m_Physics->createRigidStatic(transform);
	
	physx::PxShape* shape = m_Physics->createShape(geom, *material, true);
	physx::PxFilterData pxFOlterDate;
	pxFOlterDate.word0 = CollisionGroup::Environment;
	shape->setQueryFilterData(pxFOlterDate);
	shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	
	ground->attachShape(*shape);
	shape->release();

	m_Scene->addActor(*ground);

	return ground;
}

physx::PxController* UPhysicsSystem::CreateCapsuleController(physx::PxCapsuleControllerDesc& desc)
{
	desc.material = m_Material;

	physx::PxController* rawPtr = m_ControllerManager->createController(desc);
	if (!rawPtr)
		ThrowRuntimeError("Erro ao criar o controller.");

	m_CharacterControllers[rawPtr] = rawPtr;

	return rawPtr;
}
