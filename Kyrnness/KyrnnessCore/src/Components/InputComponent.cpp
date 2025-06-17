#include "pch.hpp"
#include "Components/InputComponent.hpp"

void FInputComponent::RegisterInputAction(const std::string& actionName, const std::function<void()>& callback)
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

nlohmann::json FInputComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FInputComponent";
	jsonData["Update"] = bCanUpdate;

	return jsonData;
}
