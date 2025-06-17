#pragma once
#ifndef KYRNNESS_COMPONENT_BUILDER_HPP
#define KYRNNESS_COMPONENT_BUILDER_HPP

#include "Core/Core.hpp"
#include "Core/MinimalComponents.hpp"
#include "nlohmann/json.hpp"
#include "Components/Component.hpp"
#include <typeindex>

class TSceneObject;

struct TComponentBuilder
{
	FShaderOpenGLComponent* defaultShader = nullptr;
	UApplication* application = nullptr;
};

class FComponentBuilder
{
	using ComponentBuilder = std::function<void(entt::registry&, entt::entity, const nlohmann::json&, TSceneObject*)>;
	static std::unordered_map<std::string, ComponentBuilder> g_ComponentBuilders;
	static TComponentBuilder g_ComponentBuilderParameters;

public:
	static TComponentBuilder& GetComponentBuilderParameters()
	{
		return g_ComponentBuilderParameters;
	}

	static std::unordered_map<std::string, ComponentBuilder>& GetComponentBuilders()
	{
		return g_ComponentBuilders;
	}
	static bool Contains(const std::string& type);
	static void Build(const std::string& type, entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* outComponent);
	static void RegisterEngineComponents(const TComponentBuilder& componentBuilderParameters);

private:
	static ComponentBuilder RegisterIdentityComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterTransformComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterCameraComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterMeshComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterInputComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterCapsuleComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterPlayerComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterBoxCollisionComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterTerrainComponent(const TComponentBuilder& componentBuilderParameters);
	static ComponentBuilder RegisterAudioSourceComponent(const TComponentBuilder& componentBuilderParameters);
};

#endif // KYRNNESS_COMPONENT_BUILDER_HPP
