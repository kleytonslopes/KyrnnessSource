/*********************************************************************
 *   File: SceneManager.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_SCENE_MANAGER_HPP
#define K_SCENE_MANAGER_HPP

#include "Class.hpp"
#include "GameFramework/Scene.hpp"

enum class ESceneState
{
    Idle,
    Transitioning,
    Finalizing
};

class USceneManager : public UClass
{
    using Super = UClass;
public:
    USceneManager(class UApplication* app);
    ~USceneManager();

    void Initialize() override;
    void OnUpdate(float deltaTime) override;
    

    // Troca de cena s�ncrona
    void SwitchScene(const std::string& scenePath, bool clearUI = true, bool clearAudio = false, bool clearLua = true);

    // Retorna ponteiro para a cena atual
    UScene* GetCurrentScene() const { return m_CurrentScene; }

protected:
    void OnDestroy() override;

    void BeginSceneTransition(UScene* nextScene);
    void FinalizeSceneTransition();

private:
    UApplication* m_Application = nullptr;
    
    ESceneState m_State = ESceneState::Idle;

    UScene* m_PreviousScene = nullptr;
    UScene* m_CurrentScene = nullptr;
    UScene* m_TransientScene = nullptr;
    UScene* m_NextScene = nullptr;
    std::string m_CurrentScenePath;

    void UnloadCurrentScene(bool clearUI, bool clearAudio, bool clearLua);
    void LoadSceneFromPath(const std::string& scenePath);
};

#endif //K_SCENE_MANAGER_HPP