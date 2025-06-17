#include "pch.hpp"
#include "Components/BoxCollisionComponent.hpp"

void FBoxCollisionComponent::Initialize()
{
	FCollisionComponent::Initialize();
}

void FBoxCollisionComponent::Update(float deltaTime)
{
	FCollisionComponent::Update(deltaTime);
}

nlohmann::json FBoxCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FBoxCollisionComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
