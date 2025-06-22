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
    ULuaManager();
    ~ULuaManager();

    void Initialize() override;
    void LoadScript(const std::string& filePath);

    sol::state& GetLuaState() { return m_LuaState; }
    ULuaEventManager& GetLuaEventManager() { return *m_LuaEventManager.get(); }

    bool CallFunction(const std::string& functionName);

protected:
    void PreInitialize() override;

    void OnInitialize() override;

private:
    FSolState m_LuaState;

    std::unique_ptr<ULuaEventManager> m_LuaEventManager;
};

#endif // KYRNESS_LUAMANAGER_HPP
