#pragma once

#ifndef KYRNNESS_MESH_COMPONENT_HPP
#define KYRNNESS_MESH_COMPONENT_HPP

#include "Components/Component.hpp"
#include "Content/MeshAsset.hpp"

class UMeshComponent : public UComponent
{
	using Super = UComponent;
public:
	UMeshComponent(const std::string& meshName, const std::string& meshFilePath);
	virtual ~UMeshComponent() = default;

	UMeshComponent(const UMeshComponent&) = delete;
	UMeshComponent& operator=(const UMeshComponent&) = delete;
	UMeshComponent(UMeshComponent&&) = delete;
	UMeshComponent& operator=(UMeshComponent&&) = delete;

	std::vector<Vertex> GetVertices() const;
	std::vector<uint32> GetIndices() const;

	FMeshAsset& GetMeshAsset() { return m_MeshAsset; }

	glm::vec3 GetBoundingBox() const { return m_MeshAsset.boundingBoxMax - m_MeshAsset.boundingBoxMin; }
	glm::vec3 GetBoundingBoxCenter() const { return m_MeshAsset.BoundingBoxCenter(); }

	virtual nlohmann::json GetJsonData() override;

protected:
	void OnInitialize() override;
	void OnDestroy() override;

private:
	std::string m_MeshName;
	std::string m_MeshFilePath;

	FMeshAsset m_MeshAsset;

	void LoadMeshFile();
};

#endif // !KYRNNESS_MESH_COMPONENT_HPP

