/*********************************************************************
 *   File: TransformComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/TransformComponent.hpp"

nlohmann::json UTransformComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UTransformComponent";
	jsonData["Location"] = { Location.x, Location.y, Location.z };
	jsonData["Rotation"] = { Rotation.x, Rotation.y, Rotation.z };
	jsonData["Scale"] = { Scale.x, Scale.y, Scale.z };
	jsonData["ForwardVector"] = { ForwardVector.x, ForwardVector.y, ForwardVector.z };
	jsonData["UpVector"] = { UpVector.x, UpVector.y, UpVector.z };
	jsonData["RightVector"] = { RightVector.x, RightVector.y, RightVector.z };
	jsonData["Update"] = bCanUpdate;

    return jsonData;
}
