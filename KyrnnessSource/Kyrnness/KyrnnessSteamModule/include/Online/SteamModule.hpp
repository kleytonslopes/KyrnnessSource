#pragma once
#ifndef KYRNESS_STEAM_MODULE_HPP
#define KYRNESS_STEAM_MODULE_HPP

class USteamModule
{
public:
	bool bNeedRestart = false;

	// Constructor
	USteamModule();

	// Destructor
	virtual ~USteamModule();

	// Initialize the Steam module
	void Initialize();

	// Shutdown the Steam module
	void Shutdown();
};
#endif // KYRNESS_STEAM_MODULE_HPP
