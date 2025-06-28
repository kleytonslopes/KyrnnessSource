#pragma once
#ifndef KYRNESS_STEAM_MODULE_HPP
#define KYRNESS_STEAM_MODULE_HPP

#include "steam/steam_api.h"

class USteamModule
{
public:
	static USteamModule& Get();

	bool bNeedRestart = false;

	// Constructor
	USteamModule();

	// Destructor
	virtual ~USteamModule();

	// Initialize the Steam module
	void Initialize();

	void CreateSession();

	void Update(float DeltaTime);

	// Shutdown the Steam module
	void Shutdown();

private:
	bool bEnabled = false;

	// Callback para criar o lobby
	STEAM_CALLBACK(USteamModule, OnLobbyCreated, LobbyCreated_t, m_CallbackLobbyCreated);

	// Callback para lobby atualizado (opcional, exemplo)
	STEAM_CALLBACK(USteamModule, OnLobbyEnter, LobbyEnter_t, m_CallbackLobbyEnter);


	void CreateAppIdFile();
	void RemoveAppIdFile();


};
#endif // KYRNESS_STEAM_MODULE_HPP
