/*********************************************************************
 *   File: TerrainComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/TerrainComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Runtime/Application.hpp"

void UTerrainComponent::OnInitialize()
{
	UTerrain::GenerateTerrain(m_Noise, m_Amplitude, m_Width, m_Height, m_TileSize, m_MeshAsset.vertices, m_MeshAsset.indices);

	UTransformComponent& transformComponent = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

	std::vector<glm::vec3> positions;
	positions.reserve(m_MeshAsset.vertices.size());

	for (const auto& vertex : m_MeshAsset.vertices)
	{
		positions.push_back(vertex.position);
	}

	auto* mesh = m_Application->GetPhysicsSystem()->CreateTriangleMeshCollision(positions, m_MeshAsset.indices, glm::vec3{ 0.f }, glm::vec3{ 0.f }, glm::vec3{ 1.f });

	Super::OnInitialize();
}

std::vector<Vertex> UTerrainComponent::GetVertices() const
{
	return m_MeshAsset.vertices;
}

std::vector<uint32> UTerrainComponent::GetIndices() const
{
	return m_MeshAsset.indices;
}

void UTerrainComponent::SetWidth(int value)
{
	m_Width = value;
}

void UTerrainComponent::SetHeight(int value)
{
	m_Height = value;
}

void UTerrainComponent::SetTileSize(float value)
{
	m_TileSize = value;
}

void UTerrainComponent::SetAmplitude(float value)
{
	m_Amplitude = value;
}

void UTerrainComponent::SetNoise(float value)
{
	m_Noise = value;
}

nlohmann::json UTerrainComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UTerrainComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["TerrainSize"] = { m_Width, m_Height };
	jsonData["TileSize"] = m_TileSize;
	jsonData["Amplitude"] = m_Amplitude;
	jsonData["Noise"] = m_Noise;

	return jsonData;
}
