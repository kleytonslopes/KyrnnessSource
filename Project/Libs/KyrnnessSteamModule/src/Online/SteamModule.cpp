#include "pch.hpp"
#include "Online/SteamModule.hpp"

#include "steam/isteamugc.h"
#include "steam/isteamgameserver.h"
#include <iostream>
#include <fstream>
#include <filesystem>

int appId = 2343940;

USteamModule& USteamModule::Get()
{
	static USteamModule Instance;
	return Instance;
}

USteamModule::USteamModule()
	: m_CallbackLobbyCreated(this, &USteamModule::OnLobbyCreated),
      m_CallbackLobbyEnter(this, &USteamModule::OnLobbyEnter)
{
}

USteamModule::~USteamModule()
{
}

void USteamModule::Initialize()
{
	if (!bEnabled)
		return;

	CreateAppIdFile();

	if (SteamAPI_RestartAppIfNecessary(appId))
	{
		bNeedRestart = true;
		RemoveAppIdFile();
		return;
	}

	if (!SteamAPI_Init())
	{
		bNeedRestart = true;
		RemoveAppIdFile();
		return;
	}

	RemoveAppIdFile();

	//SteamFriends()->ActivateGameOverlay("friends");
}

void USteamModule::CreateSession()
{
	if (!bEnabled)
		return;

	std::cout << "Tentando criar um lobby...\n";
	SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 4);
}

void USteamModule::Update(float DeltaTime)
{
	if (!bEnabled)
		return;

	SteamAPI_RunCallbacks();
}

void USteamModule::Shutdown()
{
	if (!bEnabled)
		return;

	SteamAPI_Shutdown();
	SteamAPI_ReleaseCurrentThreadMemory();
}

void USteamModule::CreateAppIdFile()
{
	if (!bEnabled)
		return;

	std::ofstream file("steam_appid.txt");
	if (file.is_open())
	{
		file << appId << std::endl;
		std::cout << "Arquivo criado com sucesso." << std::endl;
	}
	else
	{
		std::cerr << "Erro ao criar o arquivo." << std::endl;
	}
}

void USteamModule::RemoveAppIdFile()
{
	if (!bEnabled)
		return;

	if (std::filesystem::remove("steam_appid.txt"))
	{
		std::cout << "Arquivo apagado com sucesso." << std::endl;
	}
	else
	{
		std::cerr << "Falha ao apagar o arquivo." << std::endl;
	}
}

void USteamModule::OnLobbyCreated(LobbyCreated_t* pCallback)
{
	if (pCallback->m_eResult == k_EResultOK)
	{
		std::cout << "Lobby criado com sucesso! Lobby ID: " << pCallback->m_ulSteamIDLobby << "\n";
	}
	else
	{
		std::cout << "Falha ao criar lobby. Erro: " << pCallback->m_eResult << "\n";
	}

	//CSteamID steamIDLobby = SteamMatchmaking()->GetLobbyByIndex(0);


	int nDados = SteamMatchmaking()->GetLobbyDataCount(pCallback->m_ulSteamIDLobby);
	char chave[k_nMaxLobbyKeyLength];
	char valor[k_cubChatMetadataMax];
	for (int i = 0; i < nDados; ++i)
	{
		bool bSucesso = SteamMatchmaking()->GetLobbyDataByIndex(pCallback->m_ulSteamIDLobby, i, chave, k_nMaxLobbyKeyLength, valor, k_cubChatMetadataMax);
		if (bSucesso)
		{
			printf("Dados da sala %d, Chave: \"%s\" - Valor: \"%s\"\n", i, chave, valor);
		}
	}
}

void USteamModule::OnLobbyEnter(LobbyEnter_t* pCallback)
{
	std::cout << "Entrou no lobby! Lobby ID: " << pCallback->m_ulSteamIDLobby << "\n";
}
