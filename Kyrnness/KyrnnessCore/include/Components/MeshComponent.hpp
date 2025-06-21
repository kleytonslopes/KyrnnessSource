#pragma once

#ifndef KYRNNESS_MESH_COMPONENT_HPP
#define KYRNNESS_MESH_COMPONENT_HPP

#include "Components/Component.hpp"
#include "Content/MeshAsset.hpp"

class FMeshComponent : public UComponent
{
public:
	FMeshComponent(const std::string& meshName, const std::string& meshFilePath);
	virtual ~FMeshComponent() = default;

	FMeshComponent(const FMeshComponent&) = delete;
	FMeshComponent& operator=(const FMeshComponent&) = delete;
	FMeshComponent(FMeshComponent&&) = delete;
	FMeshComponent& operator=(FMeshComponent&&) = delete;

	void Initialize();
	void Destroy();

	std::vector<Vertex> GetVertices() const;
	std::vector<uint32> GetIndices() const;

	FMeshAsset& GetMeshAsset() { return m_MeshAsset; }

	glm::vec3 GetBoundingBox() const { return m_MeshAsset.boundingBoxMax - m_MeshAsset.boundingBoxMin; }
	glm::vec3 GetBoundingBoxCenter() const { return m_MeshAsset.BoundingBoxCenter(); }

	virtual nlohmann::json GetJsonData() override;
private:
	std::string m_MeshName;
	std::string m_MeshFilePath;

	FMeshAsset m_MeshAsset;

	void LoadMeshFile();
};

#endif // !KYRNNESS_MESH_COMPONENT_HPP

