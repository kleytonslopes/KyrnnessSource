#include "pch.hpp"
#include "Components/CollisionComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Runtime/Application.hpp"

void FCollisionComponent::Initialize()
{
	FComponent::Initialize();

	FTransformComponent& transformComponent = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
	FMeshComponent& meshComponent = m_Application->GetEnttRegistry().get<FMeshComponent>(m_EntityOwner);

	//m_BodyStatic = m_Application->GetPhysicsSystem()->CreateStaticBody(meshComponent.GetBoundingBoxCenter() + transformComponent.Location, meshComponent.GetBoundingBox(), transformComponent.Rotation);


	std::vector<glm::vec3> positions;
	positions.reserve(meshComponent.GetVertices().size());

	for (const auto& vertex : meshComponent.GetVertices())
	{
		positions.push_back(vertex.position);
	}

	auto* mesh = m_Application->GetPhysicsSystem()->CreateTriangleMeshCollision(positions, meshComponent.GetIndices(), transformComponent.Location, transformComponent.Rotation, transformComponent.Scale);

}

void FCollisionComponent::Update(float deltaTime)
{
}

nlohmann::json FCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FCollisionComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["Mass"] = m_Mass;
	jsonData["BoundBox"] = { m_BoundBox.x, m_BoundBox.y, m_BoundBox.z };
	jsonData["InitialInertia"] = { m_InitialInertia.x, m_InitialInertia.y, m_InitialInertia.z };

	return jsonData;
}

void FCollisionComponent::SetMass(float mass)
{
}

void FCollisionComponent::SetBoundBox(const glm::vec3& boundBox)
{
}
