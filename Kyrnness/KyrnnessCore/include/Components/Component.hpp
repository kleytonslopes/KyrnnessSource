#pragma once
#ifndef KYRNNES_COMPONENT_HPP
#define KYRNNES_COMPONENT_HPP

#include "Core/Core.hpp"
#include "nlohmann/json.hpp"

class UApplication;

class UComponent
{
public:
	entt::entity GetEntityOwner() const { return m_EntityOwner; }

	virtual void Initialize();
	virtual void Update(float deltaTime) { };

	void SetEntityOwner(entt::entity entity);
	void SetApplication(UApplication* application);
	void SetCanUpdate(bool canUpdate);

	virtual nlohmann::json GetJsonData() = 0;

protected:
	UApplication* m_Application = nullptr;
	entt::entity m_EntityOwner = entt::null;
	bool bCanUpdate = false;



	friend class UComponentBuilder;
};

#endif// KYRNNES_COMPONENT_HPP
