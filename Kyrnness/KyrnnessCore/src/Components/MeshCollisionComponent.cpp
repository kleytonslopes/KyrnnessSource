#include "pch.hpp"
#include "Components/MeshCollisionComponent.hpp"

nlohmann::json FMeshCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FMeshCollisionComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
