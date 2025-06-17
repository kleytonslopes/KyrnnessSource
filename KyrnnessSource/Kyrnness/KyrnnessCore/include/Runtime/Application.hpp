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

class UApplication
{
public:
    FFunctionEvent<float> OnUpdateEvent;

    UApplication();
    virtual ~UApplication();

    UApplication(const UApplication&) = delete;
    UApplication& operator=(const UApplication&) = delete;
    UApplication(UApplication&&) = delete;
    UApplication& operator=(UApplication&&) = delete;

    void Run();
    void Destroy();

    template<typename T>
    inline T* GetWindow() const noexcept
    {
        return static_cast<T*>(m_Window.get());
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

    UScene* GetScene() const { return m_Scene.get(); }
    UController* GetController() const { return m_Controller.get(); }
	UPhysicsSystem* GetPhysicsSystem() const { return m_PhysicsSystem.get(); }

	float GetDeltaTime() const { return m_DeltaTime; }

    EGraphicsApi GetGraphicsApiType() const;
    
	EWindowType GetWindowType() const;

    uint32 GetWidth() const;
    
    uint32 GetHeight() const;

    entt::registry& GetEnttRegistry() { return m_EnttRegistry; }



protected:
    void LoadConfiguration();

    void GameLoop();

private:
    std::unique_ptr<UWindow> m_Window;
    std::unique_ptr<UGraphicsApi> m_GraphicsApi;
	std::unique_ptr<UScene> m_Scene;
	std::unique_ptr<UController> m_Controller;
	std::unique_ptr<UPhysicsSystem> m_PhysicsSystem;

	float m_DeltaTime = 0.0f;

    entt::registry m_EnttRegistry;

    GameConfig m_GameConfig{};

    void CalculeDeltaTime(FTime& currentTime);

    void LoadGameConfiguration();
    void InitializeWindow();
	void InitializeController();
    void InitializeScene();
    void InitializeGraphicsApi();
	void InitializePhysicsSystem();
};

#endif // KYRNNES_CORE_RUNTIME_APPLICATION_HPP