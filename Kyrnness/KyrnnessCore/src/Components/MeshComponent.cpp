#include "pch.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/AssetManager.hpp"
#include "Core/Vertex.hpp"


UMeshComponent::UMeshComponent(const std::string& meshName, const std::string& meshFilePath)
	: m_MeshName(meshName), m_MeshFilePath(meshFilePath)
{
	FLogger::Log("MeshComponent created with name: " + meshName + " and file path: " + meshFilePath);
}

void UMeshComponent::OnInitialize()
{
	LoadMeshFile();

	Super::OnInitialize();
}

void UMeshComponent::OnDestroy()
{
	FLogger::Warning("MeshComponent destroyed: " + m_MeshName);

	Super::OnDestroy();
}

std::vector<Vertex> UMeshComponent::GetVertices() const
{
	return m_MeshAsset.vertices;
}

std::vector<uint32> UMeshComponent::GetIndices() const
{
	return m_MeshAsset.indices;
}

nlohmann::json UMeshComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UMeshComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["MeshName"] = m_MeshName;
	jsonData["MeshPath"] = m_MeshFilePath;

	return jsonData;
}

void UMeshComponent::LoadMeshFile()
{
	//FMeshAsset meshAsset;

	UAssetManager::LoadMeshAsset(m_MeshFilePath, m_MeshAsset);

	//m_Vertices = meshAsset.vertices;
	//m_Indices = meshAsset.indices;

}
