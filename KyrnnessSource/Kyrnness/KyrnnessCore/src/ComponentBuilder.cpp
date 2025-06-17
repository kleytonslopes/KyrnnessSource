#include "pch.hpp"
#include "ComponentBuilder.hpp"
#include "Core/Json.hpp"
#include "Runtime/Application.hpp"

using ComponentBuilder = std::function<void(entt::registry&, entt::entity, const nlohmann::json&)>;
std::unordered_map<std::string, FComponentBuilder::ComponentBuilder> FComponentBuilder::g_ComponentBuilders;
TComponentBuilder FComponentBuilder::g_ComponentBuilderParameters;

bool FComponentBuilder::Contains(const std::string& type)
{
	return g_ComponentBuilders.contains(type);
}

void FComponentBuilder::Build(const std::string& type, entt::registry& registry, entt::entity entity, const nlohmann::json& data)
{
	if (Contains(type))
		g_ComponentBuilders[type](registry, entity, data);
	else
		LOG_FATAL(TEXT("FComponentBuilder::Build: Componente desconhecido: %s", type));
}

void FComponentBuilder::RegisterEngineComponents(const TComponentBuilder& componentBuilderParameters)
{
	g_ComponentBuilders["FIdentityComponent"] = RegisterIdentityComponent(componentBuilderParameters);
	g_ComponentBuilders["FTransformComponent"] = RegisterTransformComponent(componentBuilderParameters);
	g_ComponentBuilders["FCameraComponent"] = RegisterCameraComponent(componentBuilderParameters);
	g_ComponentBuilders["FMeshComponent"] = RegisterMeshComponent(componentBuilderParameters);
	g_ComponentBuilders["FInputComponent"] = RegisterInputComponent(componentBuilderParameters);
	g_ComponentBuilders["FCapsuleComponent"] = RegisterCapsuleComponent(componentBuilderParameters);
	g_ComponentBuilders["FPlayerComponent"] = RegisterPlayerComponent(componentBuilderParameters);
	g_ComponentBuilders["FBoxCollisionComponent"] = RegisterBoxCollisionComponent(componentBuilderParameters);
	g_ComponentBuilders["FTerrainComponent"] = RegisterTerrainComponent(componentBuilderParameters);
}

ComponentBuilder FComponentBuilder::RegisterIdentityComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FIdentityComponent>(entity))
			{
				auto& comp = registry.emplace<FIdentityComponent>(entity, data["Id"]);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				if (data.contains("Id")) comp.SetId(data["Id"]);
				if (data.contains("ObjectName")) comp.SetName(data["ObjectName"]);
			}
		};
}
ComponentBuilder FComponentBuilder::RegisterTransformComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FTransformComponent>(entity))
			{
				auto& comp = registry.emplace<FTransformComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				if (data.contains("Location")) comp.Location = { data["Location"][0], data["Location"][1], data["Location"][2] };
				if (data.contains("Rotation"))comp.Rotation = { data["Rotation"][0], data["Rotation"][1], data["Rotation"][2] };
				if (data.contains("Scale")) comp.Scale = { data["Scale"][0], data["Scale"][1], data["Scale"][2] };
				if (data.contains("UpVector"))comp.UpVector = { data["UpVector"][0], data["UpVector"][1], data["UpVector"][2] };
				if (data.contains("ForwardVector"))comp.ForwardVector = { data["ForwardVector"][0], data["ForwardVector"][1], data["ForwardVector"][2] };
			}
		};
}
ComponentBuilder FComponentBuilder::RegisterCameraComponent(const TComponentBuilder& componentBuilderParameters)
{
	return  [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FCameraComponent>(entity))
			{
				auto& comp = registry.emplace<FCameraComponent>(entity);

				if (data.contains("Fov")) comp.Fov = data["Fov"];
				if (data.contains("Near")) comp.Near = data["Near"];
				if (data.contains("Far")) comp.Far = data["Far"];

				if (data.contains("LocationOffset")) comp.LocationOffset = { data["LocationOffset"][0], data["LocationOffset"][1], data["LocationOffset"][2] };

				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();
			}
		};
}
ComponentBuilder FComponentBuilder::RegisterMeshComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FMeshComponent>(entity))
			{
				auto& comp = registry.emplace<FMeshComponent>(entity, data["MeshName"], data["MeshPath"]);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();


				auto& comp2 = registry.emplace<FMeshRenderer_OpenGLComponent>(entity, comp.GetMeshAsset(), componentBuilderParameters.defaultShader); // Assuming shader is set later
				if (data.contains("Update")) comp2.SetCanUpdate(data["Update"]);
				comp2.SetEntityOwner(entity);
				comp2.SetApplication(componentBuilderParameters.application);
				comp2.Initialize();
			}
		};
}
ComponentBuilder FComponentBuilder::RegisterInputComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FInputComponent>(entity))
			{
				auto& comp = registry.emplace<FInputComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();
			}
		};
}
ComponentBuilder FComponentBuilder::RegisterCapsuleComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FCapsuleComponent>(entity))
			{
				auto& comp = registry.emplace<FCapsuleComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();
			}
		};
}
ComponentBuilder FComponentBuilder::RegisterPlayerComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FPlayerComponent>(entity))
			{
				auto& comp = registry.emplace<FPlayerComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();
			}
		};
}

ComponentBuilder FComponentBuilder::RegisterBoxCollisionComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FBoxCollisionComponent>(entity))
			{
				auto& comp = registry.emplace<FBoxCollisionComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();
			}
		};
}

ComponentBuilder FComponentBuilder::RegisterTerrainComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data)
		{
			if (!registry.any_of<FTerrainComponent>(entity))
			{
				auto& comp = registry.emplace<FTerrainComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				auto& comp2 = registry.emplace<FMeshRenderer_OpenGLComponent>(entity, comp.GetMeshAsset(), componentBuilderParameters.defaultShader); // Assuming shader is set later
				if (data.contains("Update")) comp2.SetCanUpdate(data["Update"]);
				comp2.SetEntityOwner(entity);
				comp2.SetApplication(componentBuilderParameters.application);
				comp2.Initialize();
			}
		};
}
