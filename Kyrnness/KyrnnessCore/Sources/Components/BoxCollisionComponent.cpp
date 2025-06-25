#include "CorePCH.hpp"
#include "Components/BoxCollisionComponent.hpp"

nlohmann::json UBoxCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UBoxCollisionComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
