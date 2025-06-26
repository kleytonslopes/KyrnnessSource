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
	FLogger::Information("Initializing '%s'...", typeid(*this).name());
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
	FLogger::Information("PreInitializing '%s'...", typeid(*this).name());
}

void UClass::PostInitialize()
{
	FLogger::Information("PostInitializing '%s'...", typeid(*this).name());
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
	FLogger::Warning("Destroying %s Object...", typeid(*this).name());

	if (m_State == EClassState::CS_Destroying || m_State == EClassState::CS_Destroyed)
	{
		FLogger::Error("Attempt to destroy '%s' that is already being destroyed!", typeid(*this).name());
		return;
	}

	m_State = EClassState::CS_Destroying;

	OnDestroy();
}

bool UClass::IsInitialized() const
{
	return m_State == EClassState::CS_Initialized;
}

void UClass::OnInitialize()
{
	FLogger::Information("OnInitialize %s Object!", typeid(*this).name());
}

void UClass::OnPostInitialize()
{
	FLogger::Information("OnPostInitialize %s Object!", typeid(*this).name());
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
