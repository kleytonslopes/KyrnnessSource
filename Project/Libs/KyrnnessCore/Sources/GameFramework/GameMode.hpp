/*********************************************************************
 *   File: GameMode.hpp
 *  Brief: 
 * 
 * Author: Kleyton Lopes
 *   Date: junho 2025
 * 
 * Copyright (c) 2025 Kyrnness. All rights reserved. 
 *********************************************************************/

#pragma once

#ifndef K_GAMEFRAMEWORK_GAMEMODE_HPP
#define K_GAMEFRAMEWORK_GAMEMODE_HPP

#include "Class.hpp"

class UGameMode : public UClass
{
    using Super = UClass;
public:
    UGameMode() { };
    virtual ~UGameMode() { };

    UGameMode(const UGameMode& other) = default;
    UGameMode(UGameMode&& other) noexcept = default;
    UGameMode& operator=(const UGameMode& other) = default;
    UGameMode& operator=(UGameMode&& other) noexcept = default;

protected:

private:

};

#endif // K_GAMEFRAMEWORK_GAMEMODE_HPP
