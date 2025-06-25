#include "CorePCH.hpp"
#include "Components/IdentityComponent.hpp"
#include <random>
#include <sstream>
#include <iomanip>
#include "Core/Guid.hpp"

UIdentityComponent::UIdentityComponent(const std::string& objectName) : Name(objectName)
{
	GenerateId();
}

nlohmann::json UIdentityComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UIdentityComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["Id"] = Id;
	jsonData["ObjectName"] = Name;

	return jsonData;
}

void UIdentityComponent::GenerateId()
{
	Id = FGuid::NewGuid().ToString();
}
