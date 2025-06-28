/*********************************************************************
 *   File: InputComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/InputComponent.hpp"

void UInputComponent::RegisterInputAction(const std::string& actionName, const TFunction<void()>& callback)
{
	if (m_InputActions.find(actionName) == m_InputActions.end())
	{
		m_InputActions[actionName] = callback;
	}
	else
	{
		// Handle the case where the action already exists, e.g., log a warning or replace it
		// For now, we will just replace it
		m_InputActions[actionName] = callback;
	}
}

nlohmann::json UInputComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UInputComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
