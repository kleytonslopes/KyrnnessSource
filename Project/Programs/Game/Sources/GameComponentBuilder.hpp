#pragma once
#ifndef GAME_COMPONENT_BUILDER_HPP
#define GAME_COMPONENT_BUILDER_HPP

#include "ComponentBuilder.hpp"

class TSceneObject;

class FGameComponentBuilder : public UComponentBuilder
{
	using ComponentBuilder = TFunction<void(entt::registry&, entt::entity, const nlohmann::json&, TSceneObject*)>;
public:
	static void RegisterGameComponents();

private:
	static ComponentBuilder RegisterMyPlayerComponent(const TComponentBuilder& componentBuilderParameters);
};

#endif// GAME_COMPONENT_BUILDER_HPP
