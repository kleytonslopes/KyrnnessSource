#include "pch.hpp"
#include "Components/TerrainComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Runtime/Application.hpp"

void FTerrainComponent::Initialize()
{
	UTerrain::GenerateTerrain(m_Noise, m_Amplitude, m_Width, m_Height, m_TileSize, m_MeshAsset.vertices, m_MeshAsset.indices);

	FTransformComponent& transformComponent = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);

	std::vector<glm::vec3> positions;
	positions.reserve(m_MeshAsset.vertices.size());

	for (const auto& vertex : m_MeshAsset.vertices)
	{
		positions.push_back(vertex.position);
	}

	auto* mesh = m_Application->GetPhysicsSystem()->CreateTriangleMeshCollision(positions, m_MeshAsset.indices, glm::vec3{ 0.f }, glm::vec3{ 0.f }, glm::vec3{ 1.f });
}

std::vector<Vertex> FTerrainComponent::GetVertices() const
{
	return m_MeshAsset.vertices;
}

std::vector<uint32> FTerrainComponent::GetIndices() const
{
	return m_MeshAsset.indices;
}

void FTerrainComponent::SetWidth(int value)
{
	m_Width = value;
}

void FTerrainComponent::SetHeight(int value)
{
	m_Height = value;
}

void FTerrainComponent::SetTileSize(float value)
{
	m_TileSize = value;
}

void FTerrainComponent::SetAmplitude(float value)
{
	m_Amplitude = value;
}

void FTerrainComponent::SetNoise(float value)
{
	m_Noise = value;
}

nlohmann::json FTerrainComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FTerrainComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["TerrainSize"] = { m_Width, m_Height };
	jsonData["TileSize"] = m_TileSize;
	jsonData["Amplitude"] = m_Amplitude;
	jsonData["Noise"] = m_Noise;

	return jsonData;
}
