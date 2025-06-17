#include "pch.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/AssetManager.hpp"
#include "Core/Vertex.hpp"


FMeshComponent::FMeshComponent(const std::string& meshName, const std::string& meshFilePath)
	: m_MeshName(meshName), m_MeshFilePath(meshFilePath)
{
	LOG(Log, "MeshComponent created with name: " + meshName + " and file path: " + meshFilePath);
}
void FMeshComponent::Initialize()
{
	LoadMeshFile();
}

void FMeshComponent::Destroy()
{
	LOG(Warning, "MeshComponent destroyed: " + m_MeshName);
}

std::vector<Vertex> FMeshComponent::GetVertices() const
{
	return m_MeshAsset.vertices;
}

std::vector<uint32> FMeshComponent::GetIndices() const
{
	return m_MeshAsset.indices;
}

void FMeshComponent::LoadMeshFile()
{
	//FMeshAsset meshAsset;

	UAssetManager::LoadMeshAsset(m_MeshFilePath, m_MeshAsset);

	//m_Vertices = meshAsset.vertices;
	//m_Indices = meshAsset.indices;

}
