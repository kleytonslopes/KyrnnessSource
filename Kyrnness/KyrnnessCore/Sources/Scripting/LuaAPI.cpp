/*********************************************************************
 *   File: LuaAPI.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Scripting/LuaAPI.hpp"
#include "Core/Core.hpp"
#include "Runtime/Application.hpp"
#include "Runtime/SceneManager.hpp"
#include "UI/UIManager.hpp"
#include "UI/Elements/UIElement.hpp"
#include "entt/entt.hpp"


namespace LuaAPI
{
	void RestiterAll(FSolState& lua)
	{
		lua.set_function("RegisterEvent", [](const std::string& eventName, sol::function func)
			{
				ULuaManager::Get().RegisterEventFunction(eventName, func);
			});

		
		//Audio Global Functions
		FSolNamespace audioNamespace = lua.create_named_table("Audio");
		audioNamespace.set_function("PlaySound", Audio::PlaySound);

		//Game Global Functions
		FSolNamespace gameNamespace = lua.create_named_table("Game");
		gameNamespace.set_function("QuitGame", Game::QuitGame);
		gameNamespace.set_function("getPlayer", []() -> FLuaEntity
			{
				auto& registry = UApplication::Get().GetRegistry();

				auto view = registry.view<UPlayerComponent, UTransformComponent>();
				for (auto entity : view)
				{
					return FLuaEntity(entity);
				}

				// Se n�o achar, devolve um entity inv�lido
				return FLuaEntity();
			});
		gameNamespace.set_function("LoadScene", Game::LoadScene);



		//Engine Global Functions
		FSolNamespace engineNamespace = lua.create_named_table("Engine");
		engineNamespace.set_function("Log", Engine::Log);

		//UI Global Functions
		FSolNamespace uiNamespace = lua.create_named_table("UI");
		uiNamespace.set_function("SetVisible", UI::SetVisible);
		uiNamespace.set_function("RemoveElement", UI::RemoveElement);
	}

	namespace Audio
	{
		void PlaySound(const std::string& soundName)
		{
			UApplication::Get().GetSoundManager()->PlaySound(soundName, ESoundCategory::SFX);
		}
	}

	namespace Game
	{
		void QuitGame()
		{
			UApplication::Get().QuitGame();
		}

		void LoadScene(const std::string& scenePath)
		{
			UApplication::Get().GetSceneManager()->SwitchScene(scenePath, true, false, false);
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


