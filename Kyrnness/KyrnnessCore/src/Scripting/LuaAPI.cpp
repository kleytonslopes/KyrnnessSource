#include "pch.hpp"
#include "Scripting/LuaAPI.hpp"
#include "Core/Core.hpp"
#include "Runtime/Application.hpp"
#include "UI/UIManager.hpp"
#include "UI/Elements/UIElement.hpp"

namespace LuaAPI
{
	void RestiterAll(FSolState& lua)
	{
		lua.set_function("RegisterEvent", [](const std::string& eventName, sol::function func)
			{
				UApplication::Get().GetLuaManager().GetLuaEventManager().RegisterEventFunction(eventName, func);
			});

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
		uiNamespace.set_function("RemoveElement", UI::RemoveElement);
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

		void RemoveElement(const std::string& elementName)
		{
			if (UUIElement* element = UApplication::Get().GetUIManager()->FindElementByName(elementName))
			{
				UApplication::Get().GetUIManager()->RemoveElement(element);
			}
			else
				FLogger::Warning("[LuaAPI] UI.RemoveElement: Element '%s' not found!\n", elementName.c_str());
		}
	}
}


