#include "CorePCH.hpp"
#include "Runtime/SceneManager.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Scene.hpp"
#include "Scripting/LuaManager.hpp"
#include "UI/UIManager.hpp"
#include "Audio/SoundManager.hpp"
#include "Core/Logger.hpp"

USceneManager::USceneManager(UApplication* app)
    : m_Application(app)
{
}

USceneManager::~USceneManager()
{
    UnloadCurrentScene(true, true, true);
}

void USceneManager::Initialize()
{
    Super::Initialize();

    const std::string& entryScene = m_Application->GetGameConfig().m_MainMenuMap;
    if (!entryScene.empty())
    {
        SwitchScene(entryScene, false, false, true);
    }
}

void USceneManager::OnUpdate(float deltaTime)
{
    Super::OnUpdate(deltaTime);

    if (m_CurrentScene)
    {
        m_CurrentScene->Update(deltaTime);
        m_CurrentScene->DrawScene(deltaTime);
    }
}

void USceneManager::SwitchScene(const std::string& scenePath, bool clearUI, bool clearAudio, bool clearLua)
{
    UnloadCurrentScene(clearUI, clearAudio, clearLua);
    LoadSceneFromPath(scenePath);
}

void USceneManager::UnloadCurrentScene(bool clearUI, bool clearAudio, bool clearLua)
{
    if (m_CurrentScene)
    {
        FLogger::Log("[SceneManager] Unloading scene: %s\n", m_CurrentScenePath.c_str());

        m_CurrentScene->Destroy();

        m_CurrentScene = nullptr;

    }

    if (clearUI)
    {
        m_Application->GetUIManager()->ClearAllElements();
    }

    if (clearAudio)
    {
        m_Application->GetSoundManager()->StopAll();
    }

    if (clearLua)
    {
        m_Application->GetLuaManager().ClearEvents();
    }
}

void USceneManager::LoadSceneFromPath(const std::string& scenePath)
{
    FLogger::Log("[SceneManager] Loading scene: %s\n", scenePath.c_str());

    m_CurrentScene = FMemoryManager::Allocate<UScene>(m_Application);
    m_CurrentScenePath = scenePath;

    nlohmann::json sceneJson = UAssetManager::LoadJson(scenePath);
    m_CurrentScene->SpawnEntityFromJson(sceneJson);
    m_CurrentScene->Initialize();
}