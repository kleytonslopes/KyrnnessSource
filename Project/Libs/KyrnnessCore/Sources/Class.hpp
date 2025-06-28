/*********************************************************************
 *   File: Class.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef K_CLASS_HPP
#define K_CLASS_HPP

#include "Core/Core.hpp"

enum EClassState //: uint8
{
	CS_None,
	CS_Initializing,
	CS_Initialized,
	CS_Destroying,
	CS_Destroyed,
};

class UClass
{
public:
	UClass() = default;

	bool operator==(const UClass& other) const = default;

	virtual void Initialize();
	virtual void Update(float DeltaTime);
	virtual void Destroy();
	virtual void PreInitialize();

	
	bool IsInitialized() const;

protected:
	bool bCanUpdate = true;
	EClassState m_State = EClassState::CS_None;

	virtual void OnInitialize();
	virtual void OnPostInitialize();
	virtual void OnUpdate(float DeltaTime);
	virtual void OnDestroy();
	virtual void PostInitialize();

	bool IsValid() const;



};

#endif // K_CLASS_HPP;
