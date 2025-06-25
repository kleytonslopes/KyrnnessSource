#include "CorePCH.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Runtime/Application.hpp"
#include "Runtime/WindowSDL.hpp"
#include "Runtime/WindowGFLW.hpp"
#include "ComponentBuilder.hpp"

#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"
#include "Graphics/Shaders.hpp"
#include "Input/InputManager.hpp"
#include "UI/HUD.hpp"
#include "UI/UIEditor.hpp"

#include "MemoryManager.hpp"

UApplication& UApplication::Get()
{
	static UApplication Instance;
	return Instance;
}

UApplication::UApplication()
{
}

UApplication::~UApplication()
{

}

void UApplication::Run()
{
	FLogger::Log("Initializing Application...");
	
	Initialize();

	GameLoop();
}

void UApplication::PreInitialize()
{
	Super::PreInitialize();

	UInputManager::Get().SetupApplication(this);
	LoadConfiguration();

	TComponentBuilder& comp = UComponentBuilder::GetComponentBuilderParameters();
	comp.defaultShader = UShaders::GetShader(SHADER_DEFAULT);
	UComponentBuilder::RegisterEngineComponents(comp);

	//Initialize Lua Integration
	m_LuaManager = std::make_unique<ULuaManager>();
	
	//Create Window
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

	InitializeWindow();

	//Create Sound Manager
	m_SoundManager = std::make_unique<USoundManager>();

	//Create Graphics API
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

	//Register Shaders
	UShaders::Register(FShaderAsset(SHADER_DEFAULT, "Assets/Shaders/OpenGL/vert.glsl", "Assets/Shaders/OpenGL/frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_DEBUG, "Assets/Shaders/OpenGL/debug_vert.glsl", "Assets/Shaders/OpenGL/debug_frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_UI, "Assets/Shaders/OpenGL/ui_vert.glsl", "Assets/Shaders/OpenGL/ui_frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_UI_TEXT, "Assets/Shaders/OpenGL/ui_text_vert.glsl", "Assets/Shaders/OpenGL/ui_text_frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_UI_DEBUG, "Assets/Shaders/OpenGL/debug_ui_vert.glsl", "Assets/Shaders/OpenGL/debug_ui_frag.glsl"));

	//Create Controller
	m_Controller = std::make_unique<UController>(this);

	//Create Physics System
	m_PhysicsSystem = std::make_unique<UPhysicsSystem>(this);

	////Create Scene
	//m_Scene = std::make_unique<UScene>(this);
	m_SceneManager = std::make_unique<USceneManager>(this);

	//Create UI Manager
	m_UIManager = std::make_unique<UUIManager>(this);

	//Create HUD
	if (m_HUDFactory)
		m_HUD = m_HUDFactory(this);
	else
		ThrowRuntimeError("HUD factory not set!");
}

void UApplication::OnInitialize()
{
	//m_LuaScriptLoader->LoadAndRunScript("Scripts/MainMenu.lua");
	m_LuaManager->Initialize();

	InitializeAudio();
	InitializeGraphicsApi();
	InitializeShaders();
	InitializeController();
	InitializePhysicsSystem();
	InitializeScene();
	m_UIManager->Initialize();
	InitialzieHUD();

	Super::OnInitialize();
}

void UApplication::OnPostInitialize()
{
	///UApplication::Get().GetSoundManager()->PlaySound("music", ESoundCategory::Music);

	Super::OnPostInitialize();
}

void UApplication::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	FMemoryManager::ProcessPendingDestroy();

	m_LuaManager->TriggerEvent("Application.OnUpdate", DeltaTime);

	m_GraphicsApi->DrawFrame(m_DeltaTime);

	OnUpdateEvent.Broadcast(m_DeltaTime);

	//m_Scene->Update(m_DeltaTime);
	m_SceneManager->Update(DeltaTime);
	//m_Scene->DrawScene(m_DeltaTime);

	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();
	//ImGui::NewFrame();

	m_UIManager->RenderAll();
	m_HUD->Draw(m_DeltaTime);

	m_Window->PollEvents();

	//	ImGui::Render();
	//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_GraphicsApi->EndFrame();
	m_Window->EndFrame();


	//m_Scene->ProcessEditMode();

	USoundManager::Get().SetListenerPosition(
		0, 0, 0,
		1, 0, 0,
		0.0f, 1.0f, 0.0f  // Up vector padr�o
	);


	//USoundManager::Get().SetListenerPosition(
	//	playerPos.x, playerPos.y, playerPos.z,
	//	playerForward.x, playerForward.y, playerForward.z,
	//	0.0f, 1.0f, 0.0f  // Up vector padr�o
	//);


}

void UApplication::OnDestroy()
{
	m_SceneManager->Destroy();
	m_SoundManager->Shutdown();

	FMemoryManager::Get().Cleanup();

	m_Window->Destroy();

	FLogger::Warning("UApplication::Destroy");

	Super::OnDestroy();
}

void UApplication::OnResolutionUpdated(int newWidth, int newHeght)
{
	OnResolutionUpdatedEvent.Broadcast(newWidth, newHeght);
}

void UApplication::SetupHUDFactory(TFunction<std::unique_ptr<UHUD>(UApplication*)> Factory)
{
	m_HUDFactory = std::move(Factory);
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
	return m_GameConfig.m_Width;
}

uint32 UApplication::GetHeight() const
{
	return  m_GameConfig.m_Height;
}

void UApplication::QuitGame()
{
	m_Window->SetShouldClose(true);
}

void UApplication::LoadConfiguration()
{
	LoadGameConfiguration();
}

void UApplication::GameLoop()
{
	while (m_State != EClassState::CS_Initialized)
	{
		FLogger::Log("Waiting Process Initialization...");
	}
	m_SoundManager->Update();

	const double targetFrameTime = 1.0 / m_GameConfig.m_FrameRate;

	FTime currentTime = FTime::Now();

	// Controle de FPS para o t�tulo da janela
	int frameCount = 0;
	double fpsTimer = 0.0;
	bool show_demo_window = true;
	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!m_Window->ShouldClose())
	{

		FTime frameStartTime = FTime::Now();

		CalculeDeltaTime(currentTime);
		currentTime = frameStartTime;

		Update(m_DeltaTime);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

void UApplication::InitializeAudio()
{
	//m_SoundManager = std::make_unique<USoundManager>();
	m_SoundManager->Initialize();

	m_SoundManager->SetVolume(ESoundCategory::UI, 0.5);

	m_SoundManager->LoadSound("button_hovered", "Assets/Sounds/sfx_button_hovered.wav", false, false, false);
	m_SoundManager->LoadSound("click", "Assets/Sounds/sfx_lobby_player_done.wav", false, false, false);
	m_SoundManager->LoadSound("music", "Assets/Sounds/music.wav", false, true, true);
	m_SoundManager->LoadSound("music2", "Assets/Sounds/music2.wav", false, true, true);
}

void UApplication::InitializeWindow()
{
	/*switch (m_GameConfig.m_WindowType)
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
	}*/

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
	/*m_Controller = std::make_unique<UController>(this);*/
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
	if (m_SceneManager)
	{
		m_SceneManager->Initialize();
	}
	//if (m_Scene)
	//{
	//	m_Scene->LoadFromFile(m_GameConfig.m_MainMenuMap);
	//	m_Scene->Initialize();
	//}
	//else
	//{
	//	ThrowRuntimeError("Scene not Created!");
	//}
}

void UApplication::InitializeShaders()
{
	/*UShaders::Register(FShaderAsset(SHADER_DEFAULT, "Assets/Shaders/OpenGL/vert.glsl", "Assets/Shaders/OpenGL/frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_DEBUG, "Assets/Shaders/OpenGL/debug_vert.glsl", "Assets/Shaders/OpenGL/debug_frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_UI, "Assets/Shaders/OpenGL/ui_vert.glsl", "Assets/Shaders/OpenGL/ui_frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_UI_TEXT, "Assets/Shaders/OpenGL/ui_text_vert.glsl", "Assets/Shaders/OpenGL/ui_text_frag.glsl"));
	UShaders::Register(FShaderAsset(SHADER_UI_DEBUG, "Assets/Shaders/OpenGL/debug_ui_vert.glsl", "Assets/Shaders/OpenGL/debug_ui_frag.glsl"));*/

	UShaders::Initialize();
}

void UApplication::InitializeGraphicsApi()
{
	/*switch (m_GameConfig.m_Renderer)
	{
	case EGraphicsApi::GA_OpenGL:
		m_GraphicsApi = std::make_unique<UGraphicsApi_OpenGL>(this);
		break;
	case EGraphicsApi::GA_Vulkan:
		break;
	default:
		break;
	}*/

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
	/*m_PhysicsSystem = std::make_unique<UPhysicsSystem>(this);*/
	if (m_PhysicsSystem)
	{
		m_PhysicsSystem->Initialize();
	}
	else
	{
		ThrowRuntimeError("Physics System not Created!");
	}
}

void UApplication::InitialzieHUD()
{
	if (m_HUD)
	{
		m_HUD->Initialize();
	}
	else
	{
		ThrowRuntimeError("HUD factory returned null!");
	}
}
