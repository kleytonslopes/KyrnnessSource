/*********************************************************************
 *   File: EntityManagement.cpp
 *  Brief: 
 * 
 * Author: Kleyton Lopes
 *   Date: junho 2025
 * 
 * Copyright (c) 2025 Kyrnness. All rights reserved. 
 *********************************************************************/

#include "CorePCH.hpp"
#include "EntityManagement.hpp"

UEntityManagement::UEntityManagement() {}

UEntityManagement::~UEntityManagement() {}

entt::registry& UEntityManagement::GetRegistry()
{
	return m_Registry;
}

FEntity UEntityManagement::CreateEntity()
{
	FEntity entity = m_Registry.create();
	return entity;
}

bool UEntityManagement::IsValidEntity(FEntity entity)
{
	return m_Registry.valid(entity);
}

void UEntityManagement::PreInitialize()
{
	Super::PreInitialize();
}


