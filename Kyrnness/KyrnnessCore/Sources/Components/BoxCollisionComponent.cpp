/*********************************************************************
 *   File: BoxCollisionComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/BoxCollisionComponent.hpp"

nlohmann::json UBoxCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UBoxCollisionComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
