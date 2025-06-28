#include "GamePCH.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#include <Windows.h>

#define GL_SILENCE_DEPRECATION
#include "Runtime/Application.hpp"
#include "Runtime/FrequencyThread.hpp"

#include "ComponentBuilder.hpp"
#include "GameComponentBuilder.hpp"
#include <memory>
#include "Online/SteamModule.hpp"
#include "UI/GameHUD.hpp"
#include "UI/MainMenuHUD.hpp"
#include "DebugUtils.hpp"
#include "Core/AssetManager.hpp"
#include "MemoryManager.hpp"

#if DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif // DEBUG

{
	//OpenDebugConsole();
	try
	{
		UAssetManager::InitializeGData("GameContent/Game.kpak");

		USteamModule::Get().Initialize();

		if (USteamModule::Get().bNeedRestart)
		{
			return 1;
		}

		TComponentBuilder& comp = UComponentBuilder::GetComponentBuilderParameters();
		comp.application = &UApplication::Get();

		UApplication::Get().SetHUDClass<UMainMenuHUD>();

		FFrequencyThread SteamUpdateFrequency(10.0, []() {
			USteamModule::Get().Update(0.f);
			});

		SteamUpdateFrequency.Start();

		FGameComponentBuilder::RegisterGameComponents();

		USteamModule::Get().CreateSession();

		UApplication::Get().Run();

		SteamUpdateFrequency.Stop();
		UApplication::Get().Destroy();

		USteamModule::Get().Shutdown();

		FMemoryManager::Get().Cleanup();

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
