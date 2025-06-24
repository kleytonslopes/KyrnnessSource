#include "pch.hpp"
#include "Scripting/LuaManager.hpp"
#include "Scripting/LuaAPI.hpp"
#include "Runtime/Application.hpp"
#include "Core/AssetManager.hpp"

ULuaManager& ULuaManager::Get()
{
    return UApplication::Get().GetLuaManager();
}

ULuaManager::ULuaManager()
{

}

ULuaManager::~ULuaManager()
{

}

void ULuaManager::Initialize()
{
    m_LuaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string);
    
    Super::Initialize();

    RegisterEngineTypes();

    LoadAllMods();

    FLogger::Information("[Lua] Lua State inicializado!\n");
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

void ULuaManager::LoadAllMods()
{
    const std::string modsBasePath = "Mods";

    for (const auto& modEntry : std::filesystem::directory_iterator(modsBasePath))
    {
        if (!modEntry.is_directory())
            continue;

        std::string modPath = modEntry.path().string();
        std::string myModLuaPath = modPath + "/MyMod.lua";

        if (!std::filesystem::exists(myModLuaPath))
        {
            FLogger::Warning("[Lua] Ignorando Mod: %s (MyMod.lua não encontrado)\n", modPath.c_str());
            continue;
        }

        FLogger::Log("[Lua] Carregando Mod: %s\n", modPath.c_str());

        // 1 - Primeiro, carrega todos os .lua dentro de Client/ e subpastas
        const std::string clientPath = modPath + "/Client";

        if (std::filesystem::exists(clientPath))
        {
            for (const auto& file : std::filesystem::recursive_directory_iterator(clientPath))
            {
                if (!file.is_regular_file())
                    continue;

                if (file.path().extension() == ".lua")
                {
                    LoadScript(file.path().string());
                }
            }
        }

        // 2 - Depois, carrega o MyMod.lua (entry point)
        LoadScript(myModLuaPath);

        FLogger::Log("[Lua] Mod carregado com sucesso: %s\n", modPath.c_str());
    }
}

void ULuaManager::RegisterEventFunction(const std::string& eventName, sol::function func)
{
    m_RegisteredEvents[eventName].push_back(func);
}

void ULuaManager::RegisterEngineTypes()
{
    sol::state& lua = m_LuaState;

    // Registro de FVector com construtor exposto para Lua
    lua.new_usertype<FVector>("FVector",
        sol::constructors<
        FVector(),                          // Construtor vazio (opcional)
        FVector(float, float, float)        // Construtor com 3 floats (X, Y, Z)
        >(),
        "X", &FVector::X,
        "Y", &FVector::Y,
        "Z", &FVector::Z,
        "Length", &FVector::Length
    );

    // Garantir que FVector pode ser chamado como uma função global
    lua.set_function("FVector", [](float x, float y, float z) {
        return FVector(x, y, z);
        });

    lua.new_usertype<LuaAPI::FLuaEntity>("Entity",
        "SetLocation", &LuaAPI::FLuaEntity::SetLocation,
        "GetLocation", &LuaAPI::FLuaEntity::GetLocation,
        "SetRotation", &LuaAPI::FLuaEntity::SetRotation,
        "GetRotation", &LuaAPI::FLuaEntity::GetRotation,
        "SetScale", &LuaAPI::FLuaEntity::SetScale,
        "GetScale", &LuaAPI::FLuaEntity::GetScale,
        "IsValid", &LuaAPI::FLuaEntity::IsValid
    );
}

void ULuaManager::ClearEvents()
{
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
