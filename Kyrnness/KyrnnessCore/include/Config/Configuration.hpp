#pragma once
#ifndef KYRNNES_CORE_CONFIG_CONFIGURATION_HPP
#define KYRNNES_CORE_CONFIG_CONFIGURATION_HPP

#include "Core/Core.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

struct  GameConfig
{
	uint32 m_With;
	uint32 m_Height;
	int m_Renderer;
	int m_WindowType;
	uint32 m_FrameRate = 60;
	std::string m_MainMenuMap = "";
};

class FConfig
{
public:
	static inline GameConfig LoadGameConfig()
	{
		GameConfig config{};

		std::ifstream file("Config/GameConfig.json");

		if (!file)
		{
			throw std::runtime_error("Failed to open config file: GameConfig.json");
		}

		nlohmann::json j;
		file >> j;

		config.m_With = j["Resolution"]["Width"];
		config.m_Height = j["Resolution"]["Height"];
		config.m_Renderer = j["Renderer"];
		config.m_WindowType = j["WindowType"];
		config.m_FrameRate = j["FrameRate"];
		config.m_MainMenuMap = j["MainMenuMap"];

		return config;
	}

	static inline void SaveGameConfig(const GameConfig& config)
	{
		nlohmann::json j;
		j["Resolution"]["Width"] = config.m_With;
		j["Resolution"]["Height"] = config.m_Height;
		j["Renderer"] = config.m_Renderer;
		j["WindowType"] = config.m_WindowType;
		j["FrameRate"] = config.m_FrameRate;
		j["MainMenuMap"] = config.m_MainMenuMap;

		std::ofstream file("Config/GameConfig.json");
		if (!file)
		{
			throw std::runtime_error("Failed to open config file for writing: GameConfig.json");
		}

		file << j.dump(4);
	}
};

#endif // KYRNNES_CORE_CONFIG_CONFIGURATION_HPP