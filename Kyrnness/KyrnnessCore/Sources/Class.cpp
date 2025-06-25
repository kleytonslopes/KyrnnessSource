#include "CorePCH.hpp"
#include "Class.hpp"

void UClass::Initialize()
{
	using enum EClassState;

	if (m_State == EClassState::CS_Initializing || m_State == EClassState::CS_Initialized)
	{
		FLogger::Error("The Object is already being Initialized or has already been initialized");
		return;
	}

	if (m_State == EClassState::CS_Destroying || m_State == EClassState::CS_Destroyed)
	{
		FLogger::Error("Attempt to Initialize an Object that is or has been destroyed!");
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
		FLogger::Error("Attempt to process Initialization of an Object that is or has been destroyed!");
	}

	if (m_State != EClassState::CS_Initializing)
	{
		FLogger::Error("Attempting to process Initialization of an Object that is not being Initialized!");
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
		FLogger::Error("Attempt to destroy an object that is already being destroyed!");
		return;
	}

	FLogger::Warning("Destroying Object...");

	m_State = EClassState::CS_Destroying;

	OnDestroy();
	
	m_State = EClassState::CS_Destroyed;

	FMemoryManager::SetPendingDestroy(this);
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

}
