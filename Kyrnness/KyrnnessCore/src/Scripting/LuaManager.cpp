#include "pch.hpp"
#include "Scripting/LuaManager.hpp"
#include "Scripting/LuaAPI.hpp"
#include "Runtime/Application.hpp"
#include "Core/AssetManager.hpp"

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
        sol::load_result loaded = m_LuaState.load(scriptContent, filePath);

        sol::protected_function_result result = loaded();
        if (!result.valid())
        {
            sol::error err = result;
            FLogger::Error("[Lua] Erro ao executar script %s: %s\n", filePath.c_str(), err.what());
            return;
        }

        // Procura e executa a main() do script
        sol::optional<sol::function> mainFunc = m_LuaState["main"];
        if (mainFunc && mainFunc.value().valid())
        {
            sol::protected_function_result mainResult = (*mainFunc)();
            if (!mainResult.valid())
            {
                sol::error err = mainResult;
                FLogger::Error("[Lua] Erro ao executar main() no script %s: %s\n", filePath.c_str(), err.what());
            }
            else
            {
                FLogger::Log("[Lua] Executado main() do script %s com sucesso.\n", filePath.c_str());
            }

            // Limpa a global "main" pra evitar conflito com o próximo script
            m_LuaState["main"] = sol::nil;
        }
        else
        {
            FLogger::Warning("[Lua] Script %s não possui função main().\n", filePath.c_str());
        }

        FLogger::Success("[Lua] Script carregado: %s\n", filePath.c_str());
    }
    catch (const std::exception& e)
    {
        FLogger::Error("[Lua] Erro ao carregar script %s: %s\n", filePath.c_str(), e.what());
    }

    /*try
    {
        std::string scriptContent = UAssetManager::LoadText(filePath);
        m_LuaState.script(scriptContent);

        FLogger::Success("[Lua] Script carregado: %s\n", filePath.c_str());
    }
    catch (const std::exception& e)
    {
        FLogger::Error("[Lua] Erro ao carregar script %s: %s\n", filePath.c_str(), e.what());
    }*/
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
    m_LuaEventManager = std::make_unique<ULuaEventManager>(m_LuaState);

    LuaAPI::RestiterAll(m_LuaState);
}

void ULuaManager::OnInitialize()
{
    LoadScript("Scripts/MainMenu.lua");
}
