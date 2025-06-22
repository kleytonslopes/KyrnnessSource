#include "pch.hpp"
#include "Scripting/LuaEventManager.hpp"

ULuaEventManager::ULuaEventManager(FSolState& luaState) : m_Lua(luaState)
{
}

void ULuaEventManager::RegisterEventFunction(const std::string& eventName, FSolFunction func)
{
	m_EventMap[eventName].push_back(func);
}

void ULuaEventManager::CallEvent(const std::string& eventName)
{
    auto it = m_EventMap.find(eventName);
    if (it != m_EventMap.end())
    {
        for (FSolFunction& func : it->second)
        {
            sol::protected_function_result result = func();
            if (!result.valid())
            {
                sol::error err = result;
                printf("[LuaEventManager] Error calling '%s': %s\n", eventName.c_str(), err.what());
            }
        }
    }
}
