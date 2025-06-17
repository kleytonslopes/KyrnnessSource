#include "pch.hpp"
#include "Components/Component.hpp"
#include "Runtime/Application.hpp"

void FComponent::Initialize()
{
	if (m_Application)
	{
		m_Application->OnUpdateEvent.AddListener([this](float deltaTime) {
			if (bCanUpdate)
			{
				Update(deltaTime);
			}
			});
	}
}

void FComponent::SetEntityOwner(entt::entity entity)
{
	m_EntityOwner = entity;
}

void FComponent::SetApplication(UApplication* application)
{
	m_Application = application;
}

void FComponent::SetCanUpdate(bool canUpdate)
{
	bCanUpdate = canUpdate;
}