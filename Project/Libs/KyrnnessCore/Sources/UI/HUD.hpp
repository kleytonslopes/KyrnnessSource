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

#include "Class.hpp" 


class UApplication;

class UHUD : public UClass
{
	using Super = UClass;
public:
	//UHUD(UApplication* application);
	//~UHUD() {};

	void SetHUDPath(const std::string& hudFilePath);

protected:
	void OnUpdate(float DeltaTime) override;
	std::string m_HudFilePath;
	virtual void Draw(float deltaTime);

	void PreInitialize() override;
};

#endif // K_HUD_HPP
