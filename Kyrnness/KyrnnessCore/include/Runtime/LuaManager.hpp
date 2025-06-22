#pragma once
#ifndef KYRNESS_LUAMANAGER_HPP
#define KYRNESS_LUAMANAGER_HPP

#include "Class.hpp"
#include "Core/TypesDefinition.hpp"
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

    bool CallFunction(const std::string& functionName);

protected:
    void PreInitialize() override;

    void OnInitialize() override;

private:
    FSolState m_LuaState;
};

#endif // KYRNESS_LUAMANAGER_HPP
