/*********************************************************************
 *   File: IdentityComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef K_IDENTITY_COMPONENT_HPP
#define K_IDENTITY_COMPONENT_HPP

#include "Components/Component.hpp"

class UIdentityComponent : public UComponent
{
public:
	UIdentityComponent(const std::string& objectName);
	virtual ~UIdentityComponent() = default;

	UIdentityComponent(const UIdentityComponent&) = delete;
	UIdentityComponent& operator=(const UIdentityComponent&) = delete;
	UIdentityComponent(UIdentityComponent&&) = delete;
	UIdentityComponent& operator=(UIdentityComponent&&) = delete;

	std::string GetId() const { return Id; }
	std::string GetObjectName() const { return Name; }

	void SetId(const std::string& id) { Id = std::move(id); }
	void SetName(const std::string& objectName) { Name = objectName; }

	virtual nlohmann::json GetJsonData() override;

private:
	std::string Id;
	std::string Name;

	void GenerateId();

friend class UScene;
friend class FJson;
};

#endif // !K_IDENTITY_COMPONENT_HPP
