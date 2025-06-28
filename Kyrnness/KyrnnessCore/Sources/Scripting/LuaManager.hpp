/*********************************************************************
 *   File: LuaManager.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_LUAMANAGER_HPP
#define K_LUAMANAGER_HPP

#include "Class.hpp"
#include "Core/TypesDefinition.hpp"
#include "Scripting/LuaEventManager.hpp"
#include <string>

class ULuaManager : UClass
{
    using Super = UClass;

public:
    static ULuaManager& Get();

    ULuaManager();
    ~ULuaManager();

    void Initialize() override;
    void LoadScript(const std::string& filePath);
    void LoadAllMods();
    void RegisterEventFunction(const std::string& eventName, sol::function func);
    void RegisterEngineTypes();
    void ClearEvents();

    sol::state& GetLuaState() { return m_LuaState; }
    ULuaEventManager& GetLuaEventManager() { return *m_LuaEventManager.get(); }

    bool CallFunction(const std::string& functionName);

    template<typename... Args>
    void TriggerEvent(const std::string& eventName, Args&&... args)
    {
        auto it = m_RegisteredEvents.find(eventName);
        if (it == m_RegisteredEvents.end())
            return;

        for (const auto& callback : it->second)
        {
            try
            {
                sol::protected_function pfunc = callback; // Protegido contra runtime errors Lua
                sol::protected_function_result result = pfunc(std::forward<Args>(args)...);

                if (!result.valid())
                {
                    sol::error err = result;
                    FLogger::Error("[Lua] Erro durante execu��o de evento '%s': %s\n", eventName.c_str(), err.what());
                }
            }
            catch (const sol::error& e)
            {
                FLogger::Error("[Lua] Exce��o ao chamar evento '%s': %s\n", eventName.c_str(), e.what());
            }
        }
    }

protected:
    void PreInitialize() override;

    void OnInitialize() override;

private:
    FSolState m_LuaState;
    TMap<std::string, std::vector<sol::function>> m_RegisteredEvents;
    
    std::unique_ptr<ULuaEventManager> m_LuaEventManager;
};

#endif // K_LUAMANAGER_HPP
