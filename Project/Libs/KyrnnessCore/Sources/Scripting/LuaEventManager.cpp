/*********************************************************************
 *   File: LuaEventManager.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Scripting/LuaEventManager.hpp"

ULuaEventManager::ULuaEventManager(FSolState& luaState) 
{
}

//void ULuaEventManager::CallEvent(const std::string& eventName)
//{
//    auto it = m_EventMap.find(eventName);
//    if (it != m_EventMap.end())
//    {
//        for (FSolFunction& func : it->second)
//        {
//            sol::protected_function_result result = func();
//            if (!result.valid())
//            {
//                sol::error err = result;
//                printf("[LuaEventManager] Error calling '%s': %s\n", eventName.c_str(), err.what());
//            }
//        }
//    }
//}
