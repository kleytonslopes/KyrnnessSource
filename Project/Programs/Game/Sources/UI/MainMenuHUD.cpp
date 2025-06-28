/*********************************************************************
 *   File: MainMenuHUD.cpp
 *  Brief: 
 * 
 * Author: Kleyton Lopes
 *   Date: junho 2025
 * 
 * Copyright (c) 2025 Kyrnness. All rights reserved. 
 *********************************************************************/

#include "GamePCH.hpp"
#include "MainMenuHUD.hpp"

void UMainMenuHUD::PreInitialize()
{
	SetHUDPath("Content/UI/UI_MainMenu.json");

	Super::PreInitialize();
}
