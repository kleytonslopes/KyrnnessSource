/*********************************************************************
 *   File: LuaEventManager.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef KYRNNES_SCRIPTING_LUA_EVENT_MANAGER_HPP
#define KYRNNES_SCRIPTING_LUA_EVENT_MANAGER_HPP

#include "Core/TypesDefinition.hpp"

class ULuaEventManager
{
public:
    ULuaEventManager(FSolState& luaState);

    
//    void CallEvent(const std::string& eventName);
//
//private:
//    FSolState& m_Lua;
//    TMap<std::string, std::vector<sol::function>> m_EventMap;
};

#endif // KYRNNES_SCRIPTING_LUA_EVENT_MANAGER_HPP