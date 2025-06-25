/*********************************************************************
 *   File: LuaAPI.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef KYRNNES_SCRIPTING_LUA_API_HPP
#define KYRNNES_SCRIPTING_LUA_API_HPP

#include "Core/Core.hpp"
#include "Core/MinimalComponents.hpp"
#include "Runtime/Application.hpp"

namespace LuaAPI
{

    struct FLuaEntity
    {
        entt::entity EntityHandle = entt::null;

        FLuaEntity() = default;
        FLuaEntity(entt::entity handle) : EntityHandle(handle) {}

        // Set/Get Location
        void SetLocation(const FVector& newPosition)
        {
            auto& registry = UApplication::Get().GetEnttRegistry();
            if (registry.valid(EntityHandle) && registry.all_of<UTransformComponent>(EntityHandle))
            {
                registry.get<UTransformComponent>(EntityHandle).Location = newPosition.ToGlm();
            }
        }

        FVector GetLocation() const
        {
            auto& registry = UApplication::Get().GetEnttRegistry();
            if (registry.valid(EntityHandle) && registry.all_of<UTransformComponent>(EntityHandle))
            {
                return FVector(registry.get<UTransformComponent>(EntityHandle).Location);
            }
            return FVector(0, 0, 0);
        }

        // Set/Get Rotation (em Euler degrees)
        void SetRotation(const FVector& newRotation)
        {
            auto& registry = UApplication::Get().GetEnttRegistry();
            if (registry.valid(EntityHandle) && registry.all_of<UTransformComponent>(EntityHandle))
            {
                registry.get<UTransformComponent>(EntityHandle).Rotation = newRotation.ToGlm();
            }
        }

        FVector GetRotation() const
        {
            auto& registry = UApplication::Get().GetEnttRegistry();
            if (registry.valid(EntityHandle) && registry.all_of<UTransformComponent>(EntityHandle))
            {
                return FVector(registry.get<UTransformComponent>(EntityHandle).Rotation);
            }
            return FVector(0, 0, 0);
        }

        // Set/Get Scale
        void SetScale(const FVector& newScale)
        {
            auto& registry = UApplication::Get().GetEnttRegistry();
            if (registry.valid(EntityHandle) && registry.all_of<UTransformComponent>(EntityHandle))
            {
                registry.get<UTransformComponent>(EntityHandle).Scale = newScale.ToGlm();
            }
        }

        FVector GetScale() const
        {
            auto& registry = UApplication::Get().GetEnttRegistry();
            if (registry.valid(EntityHandle) && registry.all_of<UTransformComponent>(EntityHandle))
            {
                return FVector(registry.get<UTransformComponent>(EntityHandle).Scale);
            }
            return FVector(1, 1, 1);
        }

        bool IsValid() const
        {
            auto& registry = UApplication::Get().GetEnttRegistry();
            return registry.valid(EntityHandle);
        }
    };



	void RestiterAll(FSolState& lua);
	
	namespace Audio
	{
		void PlaySound(const std::string& soundName);
	}

	namespace Game
	{
		void QuitGame();
        void LoadScene(const std::string& scenePath);
		
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