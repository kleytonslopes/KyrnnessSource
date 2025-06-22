#include "pch.hpp"
#include "Scripting/LuaAPI.hpp"
#include "Core/Core.hpp"
#include "Runtime/Application.hpp"
#include "UI/UIManager.hpp"

namespace LuaAPI
{
	void RestiterAll(FSolState& lua)
	{
		//Game Global Functions
		FSolNamespace gameNamespace = lua.create_named_table("Game");
		gameNamespace.set_function("QuitGame", Game::QuitGame);
		gameNamespace.set_function("PlaySound", Game::PlaySound);

		//Engine Global Functions
		FSolNamespace engineNamespace = lua.create_named_table("Engine");
		engineNamespace.set_function("Log", Engine::Log);

		//UI Global Functions
		FSolNamespace uiNamespace = lua.create_named_table("UI");
		uiNamespace.set_function("SetVisible", UI::SetVisible);
	}

	namespace Game
	{
		void QuitGame()
		{
			UApplication::Get().QuitGame();
		}

		void PlaySound(const std::string& soundName)
		{
			UApplication::Get().GetSoundManager()->PlaySound(soundName, ESoundCategory::SFX);
		}
	}

	namespace Engine
	{
		void Log(const std::string& message)
		{
			FLogger::Log(message);
		}
	}

	namespace UI
	{
		void SetVisible(const std::string& elementName, bool visible)
		{
			UApplication::Get().GetUIManager()->SetElementVisibility(elementName, visible);
		}
	}
}


