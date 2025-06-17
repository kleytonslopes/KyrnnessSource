#pragma once
#ifndef GAME_COMPONENT_BUILDER_HPP
#define GAME_COMPONENT_BUILDER_HPP

#include "ComponentBuilder.hpp"

class FGameComponentBuilder : public FComponentBuilder
{
	using ComponentBuilder = std::function<void(entt::registry&, entt::entity, const nlohmann::json&)>;
public:
	static void RegisterGameComponents();

private:
	static ComponentBuilder RegisterMyPlayerComponent(const TComponentBuilder& componentBuilderParameters);
};

#endif// GAME_COMPONENT_BUILDER_HPP
