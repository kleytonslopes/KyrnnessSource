#pragma once

#ifndef KYRNNES_SCRIPTING_HPP
#define KYRNNES_SCRIPTING_HPP

#include "Core/TypesDefinition.hpp"

namespace LuaAPI
{
	void RestiterAll(FSolState& lua);

	namespace Game
	{
		void QuitGame();
		void PlaySound(const std::string& soundName);
	}

	namespace Engine
	{
		void Log(const std::string& message);
	}
	namespace UI
	{
		void SetVisible(const std::string& elementName, bool visible);
		void RemoveElement(const std::string& elementName);
	}
}

#endif // KYRNNES_SCRIPTING_HPP