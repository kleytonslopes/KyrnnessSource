#include "GameComponentBuilder.hpp"
#include "Components/MyPlayerComponent.hpp"
#include "GameFramework/Scene.hpp"

using ComponentBuilder = std::function<void(entt::registry&, entt::entity, const nlohmann::json&, TSceneObject*)>;

void FGameComponentBuilder::RegisterGameComponents()
{
	UComponentBuilder::GetComponentBuilders()["FMyPlayerComponent"] = RegisterMyPlayerComponent(FGameComponentBuilder::GetComponentBuilderParameters());
}

ComponentBuilder FGameComponentBuilder::RegisterMyPlayerComponent(const TComponentBuilder& componentBuilderParameters)
{
	return [componentBuilderParameters](entt::registry& registry, entt::entity entity, const nlohmann::json& data, TSceneObject* sceneObject)
		{
			if (!registry.any_of<FMyPlayerComponent>(entity))
			{
				auto& comp = registry.emplace<FMyPlayerComponent>(entity);
				if (data.contains("Update")) comp.SetCanUpdate(data["Update"]);
				comp.SetEntityOwner(entity);
				comp.SetApplication(componentBuilderParameters.application);
				comp.Initialize();

				sceneObject->m_Components["FMyPlayerComponent"] = &comp;
			}
		};
}
