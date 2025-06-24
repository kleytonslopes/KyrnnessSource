#pragma once
#ifndef KYRNESS_SCENE_MANAGER_HPP
#define KYRNESS_SCENE_MANAGER_HPP

#include "Class.hpp"
#include "GameFramework/Scene.hpp"

class USceneManager : public UClass
{
    using Super = UClass;
public:
    USceneManager(class UApplication* app);
    ~USceneManager();

    void Initialize() override;
    void OnUpdate(float deltaTime) override;

    // Troca de cena síncrona
    void SwitchScene(const std::string& scenePath, bool clearUI = true, bool clearAudio = false, bool clearLua = true);

    // Retorna ponteiro para a cena atual
    UScene* GetCurrentScene() const { return m_CurrentScene; }

private:
    void UnloadCurrentScene(bool clearUI, bool clearAudio, bool clearLua);
    void LoadSceneFromPath(const std::string& scenePath);

private:
    UApplication* m_Application = nullptr;
    UScene* m_CurrentScene = nullptr;
    std::string m_CurrentScenePath;
};

#endif //KYRNESS_SCENE_MANAGER_HPP