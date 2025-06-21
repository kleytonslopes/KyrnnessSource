#pragma once
#ifndef KYRNNESS_TERRAIN_COMPONENT_HPP
#define KYRNNESS_TERRAIN_COMPONENT_HPP

#include "Components/Component.hpp"
#include "GameFramework/Terrain.hpp"
#include "Content/MeshAsset.hpp"

class FTerrainComponent : public UComponent
{
	using Super = UComponent;
public:
	FTerrainComponent() = default;
	virtual ~FTerrainComponent() = default;

	FTerrainComponent(const FTerrainComponent&) = delete;
	FTerrainComponent& operator=(const FTerrainComponent&) = delete;
	FTerrainComponent(FTerrainComponent&&) = delete;
	FTerrainComponent& operator=(FTerrainComponent&&) = delete;

	std::vector<Vertex> GetVertices() const;
	std::vector<uint32> GetIndices() const;

	void SetWidth(int value);
	void SetHeight(int value);
	void SetTileSize(float value);
	void SetAmplitude(float value);
	void SetNoise(float value);

	FMeshAsset& GetMeshAsset() { return m_MeshAsset; }

	virtual nlohmann::json GetJsonData() override;

private:
	int m_Width = 128;
	int m_Height = 128;
	float m_TileSize = 8.f;

	float m_Noise = 0.1f;
	float m_Amplitude = 10.0f;

	FMeshAsset m_MeshAsset;

	void OnInitialize() override;

	void LoadTerrain();
	void GenerateTerrain();
};

#endif // !KYRNNESS_TERRAIN_COMPONENT_HPP
