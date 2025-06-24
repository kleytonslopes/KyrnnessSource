#include "pch.hpp"
#include "Components/MeshCollisionComponent.hpp"

nlohmann::json UMeshCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UMeshCollisionComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
