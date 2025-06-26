/*********************************************************************
 *   File: Application.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef KYRNNES_CORE_RUNTIME_APPLICATION_HPP
#define KYRNNES_CORE_RUNTIME_APPLICATION_HPP

#include "Class.hpp"
#include "Runtime/Window.hpp"

#include "Graphics/GraphicsApi.hpp"
#include "Config/Configuration.hpp"
#include "GameFramework/Scene.hpp"
#include "GameFramework/Controller.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Audio/SoundManager.hpp"
#include "UI/HUD.hpp"
#include "UI/UIManager.hpp"
#include "Runtime/SceneManager.hpp"
#include "Scripting/LuaManager.hpp"


enum EGraphicsApi
{
	GA_OpenGL = 0,
	GA_Vulkan = 1,
};

enum EWindowType
{
	WT_GLFW = 0,
	WT_SDL = 1,
};

class UApplication : public UClass
{
	using Super = UClass;

public:
	static UApplication& Get();

	FFunctionEvent<float> OnUpdateEvent;
	FFunctionEvent<int, int> OnResolutionUpdatedEvent;

	UApplication();
	virtual ~UApplication();

	UApplication(const UApplication&) = delete;
	UApplication& operator=(const UApplication&) = delete;
	UApplication(UApplication&&) = delete;
	UApplication& operator=(UApplication&&) = delete;



	void OnResolutionUpdated(int newWidth, int newHeght);
	void Run();

	template<typename T>
	inline T* GetWindow() const noexcept
	{
		return static_cast<T*>(m_Window);
	}
	inline UWindow* GetWindow() const noexcept
	{
		return m_Window;
	}
	template<typename T>
	inline T* GetGraphicsApi() const noexcept
	{
		return static_cast<T*>(m_GraphicsApi);
	}
	inline UGraphicsApi* GetGraphicsApi() const noexcept
	{
		return m_GraphicsApi;
	}
	template<typename T>
	inline T* GetHUD() const noexcept
	{
		return static_cast<T>(m_HUD);
	}
	inline UHUD* GetHUD() const noexcept
	{
		return m_HUD;
	}

	void SetupHUDFactory(TFunction<UHUD*(UApplication*)> Factory);

	template<typename THUD>
	void SetHUDClass()
	{
		SetupHUDFactory([](UApplication* App)
			{
				return FMemoryManager::Allocate<THUD>(App);
			});
	}

	UScene* GetScene() const { return m_SceneManager->GetCurrentScene(); }
	UController* GetController() const { return m_Controller; }
	UPhysicsSystem* GetPhysicsSystem() const { return m_PhysicsSystem; }
	USoundManager* GetSoundManager() { return m_SoundManager; }
	UUIManager* GetUIManager() const { return m_UIManager; }
	USceneManager* GetSceneManager() { return m_SceneManager; }

	float GetDeltaTime() const { return m_DeltaTime; }

	EGraphicsApi GetGraphicsApiType() const;

	EWindowType GetWindowType() const;

	uint32 GetWidth() const;
	uint32 GetHeight() const;

	template<typename T>
	T GetWidth() const
	{
		return static_cast<T>(m_GameConfig.m_Width);
	}
	template<typename T>
	T GetHeight() const
	{
		return static_cast<T>(m_GameConfig.m_Height);
	}

	entt::registry& GetEnttRegistry() { return m_EnttRegistry; }
	entt::registry& GetRegistry() { return m_EnttRegistry; }

	FGameConfig& GetGameConfig() { return m_GameConfig; }

	FSolState& GetLuaState() { return m_LuaManager->GetLuaState(); }
	ULuaManager& GetLuaManager() { return *m_LuaManager; }

	void QuitGame();
protected:
	void LoadConfiguration();

	void PreInitialize() override;
	void OnInitialize() override;
	void OnPostInitialize() override;
	void OnUpdate(float DeltaTime) override;
	void OnDestroy() override;

	void GameLoop();

private:
	UWindow* m_Window = nullptr;
	UGraphicsApi* m_GraphicsApi;
	UController* m_Controller;
	UPhysicsSystem* m_PhysicsSystem;
	UHUD* m_HUD;
	USoundManager* m_SoundManager;
	UUIManager* m_UIManager;
	ULuaManager* m_LuaManager;
	USceneManager* m_SceneManager;

	TFunction<UHUD*(UApplication*)> m_HUDFactory;

	float m_DeltaTime = 0.0f;

	entt::registry m_EnttRegistry;

	FGameConfig m_GameConfig{};

	void CalculeDeltaTime(FTime& currentTime);

	void LoadGameConfiguration();
	void InitializeAudio();
	void InitializeWindow();
	void InitializeController();
	void InitializeScene();
	void InitializeShaders();
	void InitializeGraphicsApi();
	void InitializePhysicsSystem();
	void InitialzieHUD();
};

#endif // KYRNNES_CORE_RUNTIME_APPLICATION_HPP