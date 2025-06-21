#pragma once

#ifndef KYRNNESS_CLASS_HPP
#define KYRNNESS_CLASS_HPP

#include "Core/Core.hpp"

enum class EClassState //: uint8
{
	CS_None         = 0,
	CS_Initializing = 1,
	CS_Initialized  = 2,
};



class UClass
{
public:
	virtual int Initialize();
	virtual int PostInitialize();
	virtual int Update(float DeltaTime);
	virtual int Destroy();

protected:
	EClassState m_State = EClassState::CS_None;

	virtual int OnInitialize();
	virtual int OnPostInitialize();
	virtual int OnUpdate(float DeltaTime);
	virtual int OnDestroy();
};

#endif // KYRNNESS_CLASS_HPP;
