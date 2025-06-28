/*********************************************************************
 *   File: CapsuleComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"

#include "Components/CapsuleComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Scene.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Physics/Callback/IgnoreControllerFilterCallback.hpp"
#include "Graphics/GraphicsApi.hpp"

UCapsuleComponent::UCapsuleComponent()
{

}

void UCapsuleComponent::OnInitialize()
{
	if (m_Application)
	{
		UTransformComponent& transformComponent = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

		
		m_CollisionFilter.data.word0 = CollisionGroup::Player; // Grupo da capsula
		m_CollisionFilter.data.word1 = CollisionMask::Environment;    // Mascaras que o raycast pode atingir
		//m_CollisionFilter.flags = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC;

		// Posicao
		glm::vec3& location = transformComponent.Location;
		physx::PxExtendedVec3 pxLocation = physx::PxExtendedVec3{ transformComponent.Location.x, transformComponent.Location.y, transformComponent.Location.z };

		// Rotacao
		//glm::vec3& rotation = transformComponent.Rotation;
		FQuaternion glmQuat = FQuaternion(glm::radians(transformComponent.Rotation)); // converte Euler -> quat
		physx::PxQuat pxQuat(glmQuat.X, glmQuat.Z, glmQuat.Z, glmQuat.W);

		physx::PxCapsuleControllerDesc desc;
		desc.height = m_Height;
		desc.scaleCoeff = 1.f;
		desc.volumeGrowth = 1.9f;
		desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING;
		desc.radius = m_Radius;
		desc.contactOffset = 0.1f;
		desc.stepOffset = 0.01f;
		desc.upDirection = physx::PxVec3(0, 1, 0); // eixo vertical
		desc.position = pxLocation;
		desc.slopeLimit = cos(glm::radians(45.f));

		m_CapsuleController = m_Application->GetPhysicsSystem()->CreateCapsuleController(desc);

		m_TransformComponent = &transformComponent;
	}

	Super::OnInitialize();
}

void UCapsuleComponent::OnUpdate(float DeltaTime)
{
	UpdateVerticalMovement(DeltaTime);

	// Atualiza transform
	physx::PxExtendedVec3 pos = m_CapsuleController->getPosition();
	//glm::vec3 newPos(static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z));
	m_TransformComponent->Location = ToVec3(pos);

	Super::OnUpdate(DeltaTime);
}

nlohmann::json UCapsuleComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UCapsuleComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["Radius"] = m_Radius;
	jsonData["Height"] = m_Height;
	jsonData["JumpForce"] = m_JumpForce;

	return jsonData;
}

void UCapsuleComponent::UpdateVerticalMovement(float deltaTime)
{
	physx::PxSweepHit hit;
	//bool isGrounded = SweepGround(1.5f, hit);
	//
	//if(isGrounded)
	//{
	//	float desiredHeight = hit.position.y + m_Radius;
	//	physx::PxExtendedVec3 pos = m_CapsuleController->getPosition();
	//	pos.y = desiredHeight;
	//	//m_CapsuleController->setPosition(pos);
	//	Move(ToVec3(pos), 1.0f);
	//}

	// Aplica gravidade
	m_VerticalVelocity += m_Application->GetScene()->GetGravity() * deltaTime;

	// Aplica pulo (j� definido por Jump())
	glm::vec3 verticalMove(0.0f, m_VerticalVelocity * deltaTime, 0.0f);
	Move(verticalMove, 1.0f);

	// Verifica se tocou o ch�o
	if (IsOnGround())
	{
		if (m_VerticalVelocity < 0.0f)
			m_VerticalVelocity = 0.0f;

		bIsOnGround = true;
	}
	else
	{
		bIsOnGround = false;
	}
}

void UCapsuleComponent::Move(glm::vec3 direction, float scale)
{
	if (m_Application)
	{
		glm::vec3 scaledDir = direction * scale;

		physx::PxVec3 displacement(scaledDir.x, scaledDir.y, scaledDir.z);
		physx::PxControllerFilters filters;

		m_CapsuleController->move(
			displacement,
			0.001f,
			0.f,
			filters
		);
	}
}

void UCapsuleComponent::Jump()
{
	if (!m_CapsuleController) return;

	if (IsOnGround())
	{
		m_VerticalVelocity = m_JumpForce;
		bIsJumping = true;
		bIsOnGround = false;
	}
}

bool UCapsuleComponent::IsOnGround() const
{
	/*
	*/
	physx::PxControllerState state;
	m_CapsuleController->getState(state);

	return (state.collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN) != 0;

	//return (m_LastControllerState.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN) != 0;
}

bool UCapsuleComponent::CheckGround(float maxDistance, physx::PxRaycastHit& hitOut) const
{
	physx::PxExtendedVec3 origin = m_CapsuleController->getFootPosition(); // base da c�psula
	physx::PxVec3 direction = physx::PxVec3(0, -1, 0); // para baixo
	physx::PxReal distance = maxDistance;
	physx::PxVec3 startLocation{ (float)origin.x, (float)origin.y, (float)origin.z };

	physx::PxRaycastBuffer hit;
	bool hasHit = m_Application->GetPhysicsSystem()->GetScene()->raycast(startLocation, direction, distance,
		hit, physx::PxHitFlag::eDEFAULT, m_CollisionFilter);

	if (hasHit)
	{
		FLogger::Log("OnHit!");
		hitOut = hit.block;
	}

	return hasHit;


	//PxCapsuleGeometry capsule(m_CapsuleRadius, m_CapsuleHeight * 0.5f);
	//PxTransform pose = PxTransform(m_CapsuleController->getFootPosition() + PxVec3(0, 0.1f, 0));

	//PxSweepBuffer hit;
	//scene->sweep(capsule, pose, PxVec3(0, -1, 0), 0.2f, hit, PxHitFlag::eDEFAULT);
}

bool UCapsuleComponent::SweepGround(float maxDistance, physx::PxSweepHit& outHit) const
{
	if (!m_CapsuleController || !m_Application)
		return false;

	physx::PxScene* scene = m_Application->GetPhysicsSystem()->GetScene();

	// Posi��o da base da c�psula (p� do personagem)
	physx::PxExtendedVec3 footPos = m_CapsuleController->getFootPosition();
	
	physx::PxVec3 origin = physx::PxVec3(footPos.x, footPos.y , footPos.z);

	// Geometria da c�psula
	physx::PxCapsuleGeometry capsuleGeom(m_Radius, m_Height * 0.5f); // Altura � metade aqui (half-height)

	// Orienta��o vertical (X = 0, Y = 1, Z = 0)
	physx::PxQuat rotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1));

	// Dire��o do sweep (para baixo)
	physx::PxVec3 direction = physx::PxVec3(0, -1, 0);

	physx::PxSweepBuffer hitBuffer;
	physx::PxQueryFilterData m_CollisionFilterNo;
	m_CollisionFilterNo.data.word0 = CollisionGroup::Environment; // Grupo da c�psula
	//m_CollisionFilterNo.data.word1 = CollisionMask::Environment;    // M�scaras que o raycast pode atingir
	m_CollisionFilterNo.flags = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC;

	physx::PxShape* capsuleShape = nullptr;
	physx::PxRigidDynamic* actor = m_CapsuleController->getActor();
	actor->getShapes(&capsuleShape, 1); // Assume s� 1 shape
	
	physx::PxFilterData pxFOlterDate;
	pxFOlterDate.word0 = CollisionGroup::Player;

	capsuleShape->setQueryFilterData(pxFOlterDate);

	bool hit = scene->sweep(
		capsuleGeom,
		physx::PxTransform(origin, rotation),
		direction,
		maxDistance,
		hitBuffer,
		physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::ePRECISE_SWEEP,
		m_CollisionFilterNo
	);

	if (hit)
	{
		FLogger::Log("OnHit Sweep!");
		outHit = hitBuffer.block;
	}

	physx::PxVec3 p0 = origin;
	physx::PxVec3 p1 = origin + direction * maxDistance;

#ifdef DEBUG
	m_Application->GetGraphicsApi()->DrawDebugLine(glm::vec3{ p0.x, p0.y, p0.z }, glm::vec3{ p1.x, p1.y, p1.z }, glm::vec4{ 1.f, 1.f, 1.f, 1.f });
#endif // DEBUG

	physx::PxPvdSceneClient* pvdClient = m_Application->GetPhysicsSystem()->GetScene()->getScenePvdClient();
	if (!pvdClient) 
		return hit;

	const physx::PxDebugLine line = { p0, p1, physx::PxU32((int(1 * 255) << 16) | (int(1* 255) << 8) | int(1 * 255)) };

	pvdClient->drawLines(&line, 1);

	return hit;
}

