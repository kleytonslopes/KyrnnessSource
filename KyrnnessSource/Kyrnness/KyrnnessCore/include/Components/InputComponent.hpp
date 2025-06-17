#pragma once
#ifndef	KYRNNES_INPUT_COMPONENT_HPP
#define KYRNNES_INPUT_COMPONENT_HPP

#include "Components/Component.hpp"

class FInputComponent : public FComponent
{
public:
	FInputComponent() = default;
	virtual ~FInputComponent() = default;

	FInputComponent(const FInputComponent&) = delete;
	FInputComponent& operator=(const FInputComponent&) = delete;
	FInputComponent(FInputComponent&&) = delete;
	FInputComponent& operator=(FInputComponent&&) = delete;

	void RegisterInputAction(const std::string& actionName, const std::function<void()>& callback);

	//void SetupMouseMovement();

private:
	std::unordered_map<std::string, std::function<void()>> m_InputActions;
};

#endif// KYRNNESS_INPUT_COMPONENT_HPP
