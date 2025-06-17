#define GL_SILENCE_DEPRECATION
#include "Runtime/Application.hpp"
#include "Runtime/FrequencyThread.hpp"

#include "ComponentBuilder.hpp"
#include "GameComponentBuilder.hpp"
#include <memory>
#include "Online/SteamModule.hpp"
#include "UI/GameHUD.hpp"

int main(int, char**)
{

	USteamModule::Get().Initialize();

	if (USteamModule::Get().bNeedRestart)
	{
		return 1;
	}

	TComponentBuilder& comp = FComponentBuilder::GetComponentBuilderParameters();
	comp.application = &UApplication::Get();

	UApplication::Get().SetHUDClass<UGameHUD>();

	TFrequencyThread SteamUpdateFrequency(10.0, []() {
		USteamModule::Get().Update(0.f);
		});

	SteamUpdateFrequency.Start();

	FGameComponentBuilder::RegisterGameComponents();

	USteamModule::Get().CreateSession();

	//UApplication::Get().SetHUDClass(UGameHUD);
	
	UApplication::Get().Run();

	SteamUpdateFrequency.Stop();
	UApplication::Get().Destroy();
	
	USteamModule::Get().Shutdown();
	

	return 0;
}
