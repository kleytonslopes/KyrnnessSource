#include "pch.hpp"

#include "Runtime/Application.hpp"
#include "Runtime/WindowSDL.hpp"
#include "Runtime/WindowGFLW.hpp"

#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"
#include "Input/InputManager.hpp"

UApplication::UApplication()
{
}

UApplication::~UApplication()
{
}

void UApplication::Run()
{
	LOG(Log, "Initializing Application...");

	UInputManager::GetInstance().SetupApplication(this);

	LoadConfiguration();

	InitializeWindow();
	InitializeGraphicsApi();
	InitializeController();
	InitializePhysicsSystem();
	InitializeScene();

	GameLoop();
}

void UApplication::Destroy()
{
	m_Window->Destroy();

	LOG(Warning, "UApplication::Destroy");
}

EGraphicsApi UApplication::GetGraphicsApiType() const
{
	return static_cast<EGraphicsApi>(m_GameConfig.m_Renderer);
}

EWindowType UApplication::GetWindowType() const
{
	return static_cast<EWindowType>(m_GameConfig.m_WindowType);
}

uint32 UApplication::GetWidth() const
{
	return m_GameConfig.m_With;
}

uint32 UApplication::GetHeight() const
{
	return  m_GameConfig.m_Height;
}

void UApplication::LoadConfiguration()
{
	LoadGameConfiguration();
}

void UApplication::GameLoop()
{
	const double targetFrameTime = 1.0 / m_GameConfig.m_FrameRate;

	FTime currentTime = FTime::Now();

	// Controle de FPS para o título da janela
	int frameCount = 0;
	double fpsTimer = 0.0;

	while (!m_Window->ShouldClose())
	{
		FTime frameStartTime = FTime::Now();

		CalculeDeltaTime(currentTime);
		currentTime = frameStartTime;

		m_GraphicsApi->DrawFrame(m_DeltaTime);

		//std::future<void> future = std::async(std::launch::async, [&]()
			//{
				OnUpdateEvent.Broadcast(m_DeltaTime);
			//});

		// Process Game
		
		m_Scene->Update(m_DeltaTime);
		m_Scene->DrawScene(m_DeltaTime);
		m_Window->PollEvents();
		m_GraphicsApi->EndFrame();


		///FTime frameEndTime = FClock::Now();
		///float frameDuration = FClock::Duration(frameEndTime, frameStartTime);
		///
		///if (frameDuration < targetFrameTime)
		///{
		///	float sleepTime = targetFrameTime - frameDuration;
		///	std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
		///}

		///// Atualiza FPS no título a cada segundo
		///frameCount++;
		///fpsTimer += frameDuration;
		///
		///if (fpsTimer >= 1.0)
		///{
		///	const std::string title = "FPS: " + std::to_string(frameCount);
		///	m_Window->SetTitle(title.c_str());
		///
		///	fpsTimer = 0.0;
		///	frameCount = 0;
		///}
	}

	m_Window->WaitIdle();
}

void UApplication::CalculeDeltaTime(FTime& currentTime)
{
	FTime newTime = FClock::Now();
	m_DeltaTime = FClock::Duration(newTime, currentTime);
	currentTime = newTime;
}

void UApplication::LoadGameConfiguration()
{
	m_GameConfig = FConfig::LoadGameConfig();
}

void UApplication::InitializeWindow()
{
	switch (m_GameConfig.m_WindowType)
	{
	case EWindowType::WT_GLFW:
		m_Window = std::make_unique<UWindowGLFW>(this);
		break;
	case EWindowType::WT_SDL:
		m_Window = std::make_unique<UWindowSDL>(this);
		break;
	default:
		m_Window = std::make_unique<UWindowSDL>(this);
		break;
	}

	if (!m_Window)
	{
		ThrowRuntimeError("Window not Created!");
	}

	if (m_Window.get())
	{
		m_Window->Initialize();
	}
}

void UApplication::InitializeController()
{
	m_Controller = std::make_unique<UController>(this);
	if (m_Controller)
	{
		m_Controller->Initialize();
	}
	else
	{
		ThrowRuntimeError("Controller not Created!");
	}
}

void UApplication::InitializeScene()
{
	m_Scene = std::make_unique<UScene>(this);
	if (m_Scene)
	{
		m_Scene->Initialize();
	}
	else
	{
		ThrowRuntimeError("Scene not Created!");
	}
}

void UApplication::InitializeGraphicsApi()
{
	switch (m_GameConfig.m_Renderer)
	{
	case EGraphicsApi::GA_OpenGL:
		m_GraphicsApi = std::make_unique<UGraphicsApi_OpenGL>(this);
		break;
	case EGraphicsApi::GA_Vulkan:
		break;
	default:
		break;
	}

	if (!m_Window)
	{
		ThrowRuntimeError("GraphicAPI not Created!");
	}

	if (m_GraphicsApi.get())
	{
		m_GraphicsApi->Initialize();
	}
}

void UApplication::InitializePhysicsSystem()
{
	m_PhysicsSystem = std::make_unique<UPhysicsSystem>(this);
	if (m_PhysicsSystem)
	{
		m_PhysicsSystem->Initialize();
	}
	else
	{
		ThrowRuntimeError("Physics System not Created!");
	}
}
