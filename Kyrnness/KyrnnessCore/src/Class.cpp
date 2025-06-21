#include "pch.hpp"
#include "Class.hpp"

int UClass::Initialize()
{
	m_State = EClassState::CS_Initializing;

	return KYRN_SUCCESS;
}

int UClass::PostInitialize()
{
	if (m_State != EClassState::CS_Initializing)
	{
		return KYRN_FAIL;
	}

	m_State = EClassState::CS_Initialized;

	return KYRN_SUCCESS;
}

int UClass::Update(float DeltaTime)
{
	return KYRN_SUCCESS;
}

int UClass::Destroy()
{
	return KYRN_SUCCESS;
}

int UClass::OnInitialize()
{
	return KYRN_SUCCESS;
}

int UClass::OnPostInitialize()
{
	return KYRN_SUCCESS;
}

int UClass::OnUpdate(float DeltaTime)
{
	return KYRN_SUCCESS;
}

int UClass::OnDestroy()
{
	return KYRN_SUCCESS;
}
