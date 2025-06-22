#include "pch.hpp"
#include "Runtime/LuaManager.hpp"
#include "Runtime/Application.hpp"
#include "Core/AssetManager.hpp"
#include "Scripting/LuaAPI.hpp"

ULuaManager::ULuaManager()
{

}

ULuaManager::~ULuaManager()
{

}

void ULuaManager::Initialize()
{
    m_LuaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string);

    FLogger::Information("[Lua] Lua State inicializado!\n");

    Super::Initialize();
}

void ULuaManager::LoadScript(const std::string& filePath)
{
    try
    {
        std::string scriptContent = UAssetManager::LoadText(filePath);
        m_LuaState.script(scriptContent);
        FLogger::Success("[Lua] Script carregado: %s\n", filePath.c_str());
    }
    catch (const std::exception& e)
    {
        FLogger::Error("[Lua] Erro ao carregar script %s: %s\n", filePath.c_str(), e.what());
    }
}

bool ULuaManager::CallFunction(const std::string& functionName)
{
    sol::function func = m_LuaState[functionName];

    if (!func.valid())
    {
        FLogger::Error("[Lua] Função '%s' não encontrada!\n", functionName.c_str());
        return false;
    }

    sol::protected_function_result result = func();

    if (!result.valid())
    {
        sol::error err = result;
        FLogger::Error("[Lua] Erro ao chamar função '%s': %s\n", functionName.c_str(), err.what());
        return false;
    }

    return true;
}

void ULuaManager::PreInitialize()
{
    LuaAPI::RestiterAll(m_LuaState);
}

void ULuaManager::OnInitialize()
{
    LoadScript("Scripts/MainMenu.lua");
}
