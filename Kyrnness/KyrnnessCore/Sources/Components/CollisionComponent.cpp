/*********************************************************************
 *   File: CollisionComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/CollisionComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Runtime/Application.hpp"

void UCollisionComponent::OnInitialize()
{
	UTransformComponent& transformComponent = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);
	UMeshComponent& meshComponent = m_Application->GetEnttRegistry().get<UMeshComponent>(m_EntityOwner);

	std::vector<glm::vec3> positions;
	positions.reserve(meshComponent.GetVertices().size());

	for (const auto& vertex : meshComponent.GetVertices())
	{
		positions.push_back(vertex.position);
	}

	auto* mesh = m_Application->GetPhysicsSystem()->CreateTriangleMeshCollision(positions, meshComponent.GetIndices(), transformComponent.Location, transformComponent.Rotation, transformComponent.Scale);

	Super::OnInitialize();
}

nlohmann::json UCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UCollisionComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["Mass"] = m_Mass;
	jsonData["BoundBox"] = { m_BoundBox.x, m_BoundBox.y, m_BoundBox.z };
	jsonData["InitialInertia"] = { m_InitialInertia.x, m_InitialInertia.y, m_InitialInertia.z };

	return jsonData;
}

void UCollisionComponent::SetMass(float mass)
{
}

void UCollisionComponent::SetBoundBox(const glm::vec3& boundBox)
{
}
