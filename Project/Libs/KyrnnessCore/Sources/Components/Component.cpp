/*********************************************************************
 *   File: Component.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/Component.hpp"
#include "Runtime/Application.hpp"

void UComponent::OnInitialize()
{
	if (m_Application)
	{
		m_Application->OnUpdateEvent.AddListener(this, &UComponent::Update);
	}

	Super::OnInitialize();
}

void UComponent::OnDestroy()
{
	Super::OnDestroy();

	if (m_Application)
	{
		m_Application->OnUpdateEvent.RemoveListener(this, &UComponent::Update);
	}
}

void UComponent::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

void UComponent::SetEntityOwner(entt::entity entity)
{
	m_EntityOwner = entity;
}

void UComponent::SetApplication(UApplication* application)
{
	m_Application = application;
}

void UComponent::SetCanUpdate(bool canUpdate)
{
	bCanUpdate = canUpdate;
}