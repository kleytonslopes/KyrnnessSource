#include "pch.hpp"
#include "Online/SteamModule.hpp"
#include "steam/steam_api.h"
#include "steam/isteamugc.h"

USteamModule::USteamModule()
{
}

USteamModule::~USteamModule()
{
}

void USteamModule::Initialize()
{
	if (SteamAPI_RestartAppIfNecessary(480))
	{
		bNeedRestart = true;
		return;
	}

	if (!SteamAPI_Init())
	{
		bNeedRestart = true;
		return;
	}
}

void USteamModule::Shutdown()
{
	SteamAPI_Shutdown();
	SteamAPI_ReleaseCurrentThreadMemory();
}
