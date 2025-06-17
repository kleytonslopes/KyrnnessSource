#include "Runtime/Application.hpp"
#include "ComponentBuilder.hpp"
#include "GameComponentBuilder.hpp"
#include <memory>
#include "Online/SteamModule.hpp"

int main()
{
#if RELEASE
	std::unique_ptr<USteamModule> SteamModule = std::make_unique<USteamModule>();
	SteamModule.get()->Initialize();
	
	if (SteamModule.get()->bNeedRestart)
	{
		return 1;
	}
#endif

	auto App = std::make_unique<UApplication>();

	TComponentBuilder& comp = FComponentBuilder::GetComponentBuilderParameters();
	comp.application = App.get();

	FGameComponentBuilder::RegisterGameComponents();

	App->Run();

	App->Destroy();

#if RELEASE
	SteamModule.get()->Shutdown();
#endif

	return 0;
}
