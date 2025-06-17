#pragma once
#ifndef KYRNNESS_TERRAIN_COMPONENT_HPP
#define KYRNNESS_TERRAIN_COMPONENT_HPP

#include "Components/Component.hpp"
#include "GameFramework/Terrain.hpp"
#include "Content/MeshAsset.hpp"

class FTerrainComponent : public FComponent
{
public:
	FTerrainComponent() = default;
	virtual ~FTerrainComponent() = default;

	FTerrainComponent(const FTerrainComponent&) = delete;
	FTerrainComponent& operator=(const FTerrainComponent&) = delete;
	FTerrainComponent(FTerrainComponent&&) = delete;
	FTerrainComponent& operator=(FTerrainComponent&&) = delete;

	void Initialize() override;

	std::vector<Vertex> GetVertices() const;
	std::vector<uint32> GetIndices() const;

	FMeshAsset& GetMeshAsset() { return m_MeshAsset; }

private:
	int m_Width = 128;
	int m_Height = 128;
	float m_TileSize = 8.f;

	float m_Noise = 0.1f;      // Escala do Perlin (mais alto = menos detalhes)
	float m_Amplitude = 10.0f; // Altura máxima das montanhas

	FMeshAsset m_MeshAsset;

	void LoadTerrain();
	void GenerateTerrain();
};

#endif // !KYRNNESS_TERRAIN_COMPONENT_HPP
