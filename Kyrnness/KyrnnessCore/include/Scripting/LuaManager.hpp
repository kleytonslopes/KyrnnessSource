#pragma once
#ifndef KYRNESS_LUAMANAGER_HPP
#define KYRNESS_LUAMANAGER_HPP

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

    sol::state& GetLuaState() { return m_LuaState; }
    ULuaEventManager& GetLuaEventManager() { return *m_LuaEventManager.get(); }

    bool CallFunction(const std::string& functionName);

    template<typename... Args>
    void TriggerEvent(const std::string& eventName, Args&&... args)
    {
        auto it = m_RegisteredEvents.find(eventName);
        if (it == m_RegisteredEvents.end())
        {
            return;
        }

        for (const auto& callback : it->second)
        {
            try
            {
                callback(std::forward<Args>(args)...);
            }
            catch (const sol::error& e)
            {
                FLogger::Error("[Lua] Erro ao disparar evento '%s': %s\n", eventName.c_str(), e.what());
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

#endif // KYRNESS_LUAMANAGER_HPP
