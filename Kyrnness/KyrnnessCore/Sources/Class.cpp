/*********************************************************************
 *   File: Class.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Class.hpp"
#include <typeinfo>

void UClass::Initialize()
{
	using enum EClassState;

	if (m_State == EClassState::CS_Initializing || m_State == EClassState::CS_Initialized)
	{
		FLogger::Error("The '%s' already being Initialized or has already been initialized", typeid(*this).name());
		return;
	}

	if (m_State == EClassState::CS_Destroying || m_State == EClassState::CS_Destroyed)
	{
		FLogger::Error("Attempt to Initialize '%s' that is or has been destroyed!", typeid(*this).name());
		return;
	}

	m_State = EClassState::CS_Initializing;

	PreInitialize();

	OnInitialize();

	PostInitialize();

	m_State = EClassState::CS_Initialized;
}

void UClass::PreInitialize()
{

}

void UClass::PostInitialize()
{
	using enum EClassState;

	if (m_State == EClassState::CS_Destroying || m_State == EClassState::CS_Destroyed)
	{
		FLogger::Error("Attempt to process Initialization of '%s' that is or has been destroyed!", typeid(*this).name());
	}

	if (m_State != EClassState::CS_Initializing)
	{
		FLogger::Error("Attempting to process Initialization of '%s' that is not being Initialized!", typeid(*this).name());
	}

	OnPostInitialize();
}

bool UClass::IsValid() const
{
	if (m_State == EClassState::CS_Initialized)
		return true;

	return false;
}

void UClass::Update(float DeltaTime)
{
	if (m_State == EClassState::CS_Destroying || m_State == EClassState::CS_Destroyed)
		return;

	if (!bCanUpdate)
		return;

	if (m_State != EClassState::CS_Initialized)
		return;

	OnUpdate(DeltaTime);
}

void UClass::Destroy()
{
	if (m_State == EClassState::CS_Destroying || m_State == EClassState::CS_Destroyed)
	{
		FLogger::Error("Attempt to destroy '%s' that is already being destroyed!", typeid(*this).name());
		return;
	}

	FLogger::Warning("Destroying %s Object...", typeid(*this).name());

	m_State = EClassState::CS_Destroying;

	OnDestroy();
}

void UClass::OnInitialize()
{
}

void UClass::OnPostInitialize()
{
}

void UClass::OnUpdate(float DeltaTime)
{
}

void UClass::OnDestroy()
{
	FMemoryManager::SetPendingDestroy(this);
	FLogger::Warning("%s Destroyed!", typeid(*this).name());
	m_State = EClassState::CS_Destroyed;
}
