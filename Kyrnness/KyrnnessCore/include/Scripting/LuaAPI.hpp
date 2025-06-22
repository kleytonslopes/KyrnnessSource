#pragma once

#ifndef KYRNNES_SCRIPTING_LUA_API_HPP
#define KYRNNES_SCRIPTING_LUA_API_HPP

#include "Core/Core.hpp"
#include "Core/MinimalComponents.hpp"

namespace LuaAPI
{
	/*struct FLuaEntity
	{
		entt::entity EntityHandle;
		entt::registry* Registry;

		FLuaEntity(entt::entity handle, entt::registry* reg)
			: EntityHandle(handle), Registry(reg) {}

		void SetPosition(float x, float y, float z)
		{
			if (Registry && Registry->valid(EntityHandle) && Registry->all_of<UTransformComponent>(EntityHandle))
			{
				auto& transform = Registry->get<UTransformComponent>(EntityHandle);
				transform.Location = FVector(x, y, z);
			}
		}

		FVector GetPosition() const
		{
			if (Registry && Registry->valid(EntityHandle) && Registry->all_of<UTransformComponent>(EntityHandle))
			{
				return Registry->get<UTransformComponent>(EntityHandle).Location;
			}
			return FVector(0, 0, 0);
		}
	};*/


	void RestiterAll(FSolState& lua);
	
	namespace Audio
	{
		void PlaySound(const std::string& soundName);
	}

	namespace Game
	{
		void QuitGame();
		
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

#endif // KYRNNES_SCRIPTING_LUA_API_HPP