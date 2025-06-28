/*********************************************************************
 *   File: MeshCollisionComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/MeshCollisionComponent.hpp"

nlohmann::json UMeshCollisionComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UMeshCollisionComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
