#pragma once

#ifndef KYRNNESS_IDENTITY_COMPONENT_HPP
#define KYRNNESS_IDENTITY_COMPONENT_HPP

#include "Components/Component.hpp"

class FIdentityComponent : public FComponent
{
public:
	FIdentityComponent(const std::string& objectName);
	virtual ~FIdentityComponent() = default;

	FIdentityComponent(const FIdentityComponent&) = delete;
	FIdentityComponent& operator=(const FIdentityComponent&) = delete;
	FIdentityComponent(FIdentityComponent&&) = delete;
	FIdentityComponent& operator=(FIdentityComponent&&) = delete;

	std::string GetId() const { return Id; }
	std::string GetObjectName() const { return Name; }

	void SetId(const std::string& id) { Id = std::move(id); }
	void SetName(const std::string& objectName) { Name = objectName; }

private:
	std::string Id;
	std::string Name;

	void GenerateId();

friend class UScene; // Allow UScene to access private members
friend class FJson; // Allow FJson to access private members
};

#endif // !KYRNNESS_IDENTITY_COMPONENT_HPP
