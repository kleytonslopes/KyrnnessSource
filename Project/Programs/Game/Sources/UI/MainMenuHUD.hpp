/*********************************************************************
 *   File: MainMenuHUD.hpp
 *  Brief: 
 * 
 * Author: Kleyton Lopes
 *   Date: junho 2025
 * 
 * Copyright (c) 2025 Kyrnness. All rights reserved. 
 *********************************************************************/

#pragma once

#ifndef K_UI_MAINMENUHUD_HPP
#define K_UI_MAINMENUHUD_HPP

#include "UI/HUD.hpp"

class UMainMenuHUD : public UHUD
{
    using Super = UHUD;
public:
    UMainMenuHUD() { }
    ~UMainMenuHUD() { }

    UMainMenuHUD(const UMainMenuHUD& other) = default;
    UMainMenuHUD(UMainMenuHUD&& other) noexcept = default;
    UMainMenuHUD& operator=(const UMainMenuHUD& other) = default;
    UMainMenuHUD& operator=(UMainMenuHUD&& other) noexcept = default;

protected:
    void PreInitialize() override;

private:

};

#endif // K_UI_MAINMENUHUD_HPP
