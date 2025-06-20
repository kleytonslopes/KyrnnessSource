#pragma once

#ifndef KYRNNES_CORE_RUNTIME_APPLICATION_HPP
#define KYRNNES_CORE_RUNTIME_APPLICATION_HPP

#include "Core/Core.hpp"
#include "Runtime/Window.hpp"
#include "Graphics/GraphicsApi.hpp"
#include "Config/Configuration.hpp"
#include "GameFramework/Scene.hpp"
#include "GameFramework/Controller.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Audio/SoundManager.hpp"
#include "UI/HUD.hpp"

enum EGraphicsApi
{
    GA_OpenGL = 0,
    GA_Vulkan = 1,
};

enum EWindowType
{
    WT_GLFW = 0,
    WT_SDL  = 1,
};

class UApplication : public UClass
{
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

    void Initialize() override;
    void PostInitialize() override;
    void Update(float DeltaTime) override;
    void Destroy() override;
    void OnResolutionUpdated(int newWidth, int newHeght);
    void Run();

    template<typename T>
    inline T* GetWindow() const noexcept
    {
        return static_cast<T*>(m_Window.get());
    }
    inline UWindow* GetWindow() const noexcept
    {
        return m_Window.get();
    }
    template<typename T>
    inline T* GetGraphicsApi() const noexcept
    {
        return static_cast<T*>(m_GraphicsApi.get());
    }
    inline UGraphicsApi* GetGraphicsApi() const noexcept
    {
        return m_GraphicsApi.get();
    }
    template<typename T>
    inline T* GetHUD() const noexcept
    {
        return static_cast<T>(m_HUD);
    }
    inline UHUD* GetHUD() const noexcept
    {
        return m_HUD.get();
    }

    void SetupHUDFactory(std::function<std::unique_ptr<UHUD>(UApplication*)> Factory);
   
    template<typename THUD>
    void SetHUDClass()
    {
        SetupHUDFactory([](UApplication* App) 
            {
                return std::make_unique<THUD>(App);
            });
    }

    UScene* GetScene() const { return m_Scene.get(); }
    UController* GetController() const { return m_Controller.get(); }
	UPhysicsSystem* GetPhysicsSystem() const { return m_PhysicsSystem.get(); }
    USoundManager* GetSoundManager() { return m_SoundManager.get(); }

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

    GameConfig& GetGameConfig() { return m_GameConfig; }

protected:
    void LoadConfiguration();

    void GameLoop();

private:
    std::unique_ptr<UWindow> m_Window;
    std::unique_ptr<UGraphicsApi> m_GraphicsApi;
	std::unique_ptr<UScene> m_Scene;
	std::unique_ptr<UController> m_Controller;
	std::unique_ptr<UPhysicsSystem> m_PhysicsSystem;
    std::unique_ptr<UHUD> m_HUD;
    std::unique_ptr<USoundManager> m_SoundManager;

    std::function<std::unique_ptr<UHUD>(UApplication*)> m_HUDFactory;

	float m_DeltaTime = 0.0f;

    entt::registry m_EnttRegistry;

    GameConfig m_GameConfig{};

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