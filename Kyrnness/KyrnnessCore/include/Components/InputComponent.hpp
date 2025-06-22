#pragma once
#ifndef	KYRNNES_INPUT_COMPONENT_HPP
#define KYRNNES_INPUT_COMPONENT_HPP

#include "Components/Component.hpp"

class UInputComponent : public UComponent
{
public:
	UInputComponent() = default;
	virtual ~UInputComponent() = default;

	UInputComponent(const UInputComponent&) = delete;
	UInputComponent& operator=(const UInputComponent&) = delete;
	UInputComponent(UInputComponent&&) = delete;
	UInputComponent& operator=(UInputComponent&&) = delete;

	void RegisterInputAction(const std::string& actionName, const TFunction<void()>& callback);

	virtual nlohmann::json GetJsonData() override;

private:
	TMap<std::string, TFunction<void()>> m_InputActions;
};

#endif// KYRNNESS_INPUT_COMPONENT_HPP
