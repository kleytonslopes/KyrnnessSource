#pragma once
#ifndef KYRNNES_CORE_CONFIG_CONFIGURATION_HPP
#define KYRNNES_CORE_CONFIG_CONFIGURATION_HPP

#include "Core/Core.hpp"
#include "Core/AssetManager.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

struct  FGameConfig
{
	uint32 m_Width;
	uint32 m_Height;
	int m_Renderer;
	int m_WindowType;
	uint32 m_FrameRate = 60;
	std::string m_MainMenuMap = "";
	int m_FontAtlasSize = 512;
};

class FConfig
{
public:
	static inline FGameConfig LoadGameConfig()
	{
		FGameConfig config{};

		try
		{
			std::vector<uint8_t> jsonData = UAssetManager::LoadAssetRaw("Config/GameConfig.json");
			std::string jsonStr(jsonData.begin(), jsonData.end());

			nlohmann::json j = nlohmann::json::parse(jsonStr);

			config.m_Width = j["Resolution"]["Width"];
			config.m_Height = j["Resolution"]["Height"];
			config.m_Renderer = j["Renderer"];
			config.m_WindowType = j["WindowType"];
			config.m_FrameRate = j["FrameRate"];
			config.m_MainMenuMap = j["MainMenuMap"];
			config.m_FontAtlasSize = j["FontAtlasSize"];
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Falha ao carregar GameConfig.json: ") + e.what());
		}

		return config;
	}

	static inline void SaveGameConfig(const FGameConfig& config)
	{
		nlohmann::json j;
		j["Resolution"]["Width"] = config.m_Width;
		j["Resolution"]["Height"] = config.m_Height;
		j["Renderer"] = config.m_Renderer;
		j["WindowType"] = config.m_WindowType;
		j["FrameRate"] = config.m_FrameRate;
		j["MainMenuMap"] = config.m_MainMenuMap;
		j["FontAtlasSize"] = config.m_FontAtlasSize;

		std::ofstream file("Config/GameConfig.json");
		if (!file)
		{
			throw std::runtime_error("Failed to open config file for writing: GameConfig.json");
		}

		file << j.dump(4);
	}
};

#endif // KYRNNES_CORE_CONFIG_CONFIGURATION_HPP