#pragma once
#ifndef KYRNNES_COMPONENT_HPP
#define KYRNNES_COMPONENT_HPP

#include "Class.hpp"
#include "nlohmann/json.hpp"

class UApplication;

class UComponent : public UClass
{
	using Super = UClass;

public:
	entt::entity GetEntityOwner() const { return m_EntityOwner; }
	
	void SetEntityOwner(entt::entity entity);
	void SetApplication(UApplication* application);
	void SetCanUpdate(bool canUpdate);

	virtual nlohmann::json GetJsonData() = 0;

protected:
	UApplication* m_Application = nullptr;
	entt::entity m_EntityOwner = entt::null;

	void OnInitialize() override;

	friend class UComponentBuilder;
};

#endif// KYRNNES_COMPONENT_HPP
