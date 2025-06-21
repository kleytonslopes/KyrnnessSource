#include "pch.hpp"
#include "Components/BoxCollisionComponent.hpp"

nlohmann::json FBoxCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FBoxCollisionComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
