/*********************************************************************
 *   File: HUD.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_HUD_HPP
#define K_HUD_HPP

#include "Core/Core.hpp" 


class UApplication;

class UHUD
{
public:
	UHUD(UApplication* application);
	~UHUD() {};

	virtual void Initialize();
	virtual void PostInitialize();

	virtual void Draw(float deltaTime);

	///UUIManager* GetUIManager() const { return m_UIManager.get(); }

protected:
	UApplication* m_Application;
};

#endif // K_HUD_HPP
