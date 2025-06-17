#include "pch.hpp"
#include "Components/IdentityComponent.hpp"
#include <random>
#include <sstream>
#include <iomanip>
#include "Core/Guid.hpp"

FIdentityComponent::FIdentityComponent(const std::string& objectName) : Name(objectName)
{
	GenerateId();
}

nlohmann::json FIdentityComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FIdentityComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["Id"] = Id;
	jsonData["ObjectName"] = Name;

	return jsonData;
}

void FIdentityComponent::GenerateId()
{
	Id = FGuid::NewGuid().ToString();
}
