#include "CorePCH.hpp"
#include "ComponentBuilder.hpp"
#include "Core/Json.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Scene.hpp"


using ComponentBuilder = TFunction<void(entt::registry&, entt::entity, const nlohmann::json&, TSceneObject*)>;
TMap<std::string, UComponentBuilder::ComponentBuilder> UComponentBuilder::g_ComponentBuilders;
TComponentBuilder UComponentBuilder::g_ComponentBuilderParameters;

bool UComponentBuilder::Contains(const std::string& type)
{
	return g_ComponentBuilders.contains(type);
}

void UComponentBuilder::Build(const std::string& type, entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
{
	if (Contains(type))
		g_ComponentBuilders[type](registry, entity, data, sceneObject);
	else
		FLogger::Fatal("UComponentBuilder::Build: Componente desconhecido: %s", type);
}

void UComponentBuilder::RegisterEngineComponents(const TComponentBuilder& componentBuilderParameters)
{
	g_ComponentBuilders["UIdentityComponent"] = RegisterIdentityComponent(componentBuilderParameters);
	g_ComponentBuilders["UTransformComponent"] = RegisterTransformComponent(componentBuilderParameters);
	g_ComponentBuilders["UCameraComponent"] = RegisterCameraComponent(componentBuilderParameters);
	g_ComponentBuilders["UMeshComponent"] = RegisterMeshComponent(componentBuilderParameters);
	g_ComponentBuilders["UInputComponent"] = RegisterInputComponent(componentBuilderParameters);
	g_ComponentBuilders["UCapsuleComponent"] = RegisterCapsuleComponent(componentBuilderParameters);
	g_ComponentBuilders["UPlayerComponent"] = RegisterPlayerComponent(componentBuilderParameters);
	g_ComponentBuilders["UBoxCollisionComponent"] = RegisterBoxCollisionComponent(componentBuilderParameters);
	g_ComponentBuilders["UTerrainComponent"] = RegisterTerrainComponent(componentBuilderParameters);
	g_ComponentBuilders["UAudioSourceComponent"] = RegisterAudioSourceComponent(componentBuilderParameters);
}

ComponentBuilder UComponentBuilder::RegisterIdentityComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UIdentityComponent>(entity))
			{
				auto& comp = registry.emplace<UIdentityComponent>(entity, data["Id"]);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				if (data.contains("Id")) comp.SetId(data["Id"]);
				if (data.contains("ObjectName")) comp.SetName(data["ObjectName"]);

				sceneObject->m_Components["UIdentityComponent"] = &comp;
			}
		};
}
ComponentBuilder UComponentBuilder::RegisterTransformComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UTransformComponent>(entity))
			{
				auto& comp = registry.emplace<UTransformComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				if (data.contains("Location")) comp.Location = { data["Location"][0], data["Location"][1], data["Location"][2] };
				if (data.contains("Rotation"))comp.Rotation = { data["Rotation"][0], data["Rotation"][1], data["Rotation"][2] };
				if (data.contains("Scale")) comp.Scale = { data["Scale"][0], data["Scale"][1], data["Scale"][2] };
				if (data.contains("UpVector"))comp.UpVector = { data["UpVector"][0], data["UpVector"][1], data["UpVector"][2] };
				if (data.contains("ForwardVector"))comp.ForwardVector = { data["ForwardVector"][0], data["ForwardVector"][1], data["ForwardVector"][2] };

				sceneObject->m_Components["UTransformComponent"] = &comp;
			}
		};
}
ComponentBuilder UComponentBuilder::RegisterCameraComponent(const TComponentBuilder& componentBuilderParameters)
{
	return  [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UCameraComponent>(entity))
			{
				auto& comp = registry.emplace<UCameraComponent>(entity);

				if (data.contains("Fov")) comp.Fov = data["Fov"];
				if (data.contains("Near")) comp.Near = data["Near"];
				if (data.contains("Far")) comp.Far = data["Far"];

				if (data.contains("LocationOffset")) comp.LocationOffset = { data["LocationOffset"][0], data["LocationOffset"][1], data["LocationOffset"][2] };

				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				sceneObject->m_Components["UCameraComponent"] = &comp;
			}
		};
}
ComponentBuilder UComponentBuilder::RegisterMeshComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UMeshComponent>(entity))
			{
				auto& comp = registry.emplace<UMeshComponent>(entity, data["MeshName"], data["MeshPath"]);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();


				auto& comp2 = registry.emplace<UMeshRenderer_OpenGLComponent>(entity, comp.GetMeshAsset(), componentBuilderParameters.defaultShader); // Assuming shader is set later
				if (data.contains("Update")) comp2.SetCanUpdate(data["Update"]);
				comp2.SetEntityOwner(entity);
				comp2.SetApplication(componentBuilderParameters.application);
				comp2.Initialize();

				sceneObject->m_Components["UMeshComponent"] = &comp;
			}
		};
}
ComponentBuilder UComponentBuilder::RegisterInputComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UInputComponent>(entity))
			{
				auto& comp = registry.emplace<UInputComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				sceneObject->m_Components["UInputComponent"] = &comp;
			}
		};
}
ComponentBuilder UComponentBuilder::RegisterCapsuleComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UCapsuleComponent>(entity))
			{
				auto& comp = registry.emplace<UCapsuleComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				sceneObject->m_Components["UCapsuleComponent"] = &comp;
			}
		};
}
ComponentBuilder UComponentBuilder::RegisterPlayerComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UPlayerComponent>(entity))
			{
				auto& comp = registry.emplace<UPlayerComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				sceneObject->m_Components["UPlayerComponent"] = &comp;
			}
		};
}

ComponentBuilder UComponentBuilder::RegisterBoxCollisionComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UBoxCollisionComponent>(entity))
			{
				auto& comp = registry.emplace<UBoxCollisionComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				sceneObject->m_Components["UBoxCollisionComponent"] = &comp;
			}
		};
}

ComponentBuilder UComponentBuilder::RegisterTerrainComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UTerrainComponent>(entity))
			{
				auto& comp = registry.emplace<UTerrainComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);

				if (data.contains("TerrainSize")) { comp.SetWidth(data["TerrainSize"][0]), comp.SetHeight(data["TerrainSize"][1]); };
				if (data.contains("TileSize")) comp.SetTileSize(data["TileSize"]);
				if (data.contains("Amplitude")) comp.SetAmplitude(data["Amplitude"]);
				if (data.contains("Noise")) comp.SetNoise(data["Noise"]);

				comp.Initialize();


				auto& comp2 = registry.emplace<UMeshRenderer_OpenGLComponent>(entity, comp.GetMeshAsset(), componentBuilderParameters.defaultShader); // Assuming shader is set later
				if (data.contains("Update")) comp2.SetCanUpdate(data["Update"]);
				comp2.SetEntityOwner(entity);
				comp2.SetApplication(componentBuilderParameters.application);
				comp2.Initialize();

				sceneObject->m_Components["UTerrainComponent"] = &comp;
			}
		};
}

ComponentBuilder UComponentBuilder::RegisterAudioSourceComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<UAudioSourceComponent>(entity))
			{
				auto& comp = registry.emplace<UAudioSourceComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				if (data.contains("Loop")) comp.SetLoop(data["Loop"]);
				if (data.contains("Volume")) comp.SetVolume(data["Volume"]);
				if (data.contains("SoundName")) comp.SetSoundName(data["SoundName"]);
				if (data.contains("Position")) comp.SetPosition({data["Position"][0], data["Position"][1], data["Position"][2]});

				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				sceneObject->m_Components["UAudioSourceComponent"] = &comp;
			}
		};
}
