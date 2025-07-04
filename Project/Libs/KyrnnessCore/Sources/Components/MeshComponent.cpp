/*********************************************************************
 *   File: MeshComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
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

void UMeshComponent::SetShaderName(const std::string& shaderName)
{
	m_MeshAsset.shaderName = shaderName;
}

nlohmann::json UMeshComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UMeshComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["MeshName"] = m_MeshName;
	jsonData["MeshPath"] = m_MeshFilePath;
	jsonData["ShaderName"] = m_MeshAsset.shaderName;

	return jsonData;
}

void UMeshComponent::LoadMeshFile()
{
	//FMeshAsset meshAsset;

	UAssetManager::LoadMeshAsset(m_MeshFilePath, m_MeshAsset);

	//m_Vertices = meshAsset.vertices;
	//m_Indices = meshAsset.indices;

}
