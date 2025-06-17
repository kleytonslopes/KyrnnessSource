#include "pch.hpp"
#include "GameFramework/Scene.hpp"
#include "GameFramework/Controller.hpp"
#include "Runtime/Application.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Components/IdentityComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/MeshRenderer_OpenGLComponent.hpp"
#include "Core/AssetManager.hpp"
#include "Core/Json.hpp"
#include "Input/InputKey.hpp"
#include "Input/InputManager.hpp"
#include "ComponentBuilder.hpp"
#include "Graphics/RenderParameters.hpp"

UScene::UScene(UApplication* application)
	: m_Application(application)
{
}

UScene::~UScene()
{
}

void UScene::Initialize()
{
	SetupInputActions();
	CreateShaders();

	TComponentBuilder& comp = FComponentBuilder::GetComponentBuilderParameters();
	comp.defaultShader = m_DefaultShader;

	FComponentBuilder::RegisterEngineComponents(comp);

	nlohmann::json sceneJson = UAssetManager::LoadJson("Content/Maps/EntryMap.json");

	SpawnEntityFromJson(sceneJson);
}

void UScene::Update(float deltaTime)
{
	for (auto& [key, action] : m_InputActions)
	{
		action(deltaTime);
	}
}

void UScene::DrawScene(float deltaTime)
{
	auto& registry = m_Application->GetEnttRegistry();

	if (registry.valid(m_MainCameraEntity))
	{

		auto objectsScene = registry.view<FTransformComponent, FMeshRenderer_OpenGLComponent>();

		FCameraComponent& cameraScene = registry.get<FCameraComponent>(m_MainCameraEntity);

		cameraScene.UpdateAspectRatio(m_Application->GetWidth(), m_Application->GetHeight());

		objectsScene.each([&](const auto entity, auto& transform, auto& meshRenderer)
			{
				TRenderParameters renderParameters
				{
					.location = transform.Location,
					.rotation = transform.Rotation,
					.scale = transform.Scale,
					.viewMatrix = cameraScene.GetViewMatrix(),
					.projectionMatrix = cameraScene.GetProjectionMatrix(),
				};
				meshRenderer.Render(renderParameters);
			});
	}
}

void UScene::MakeScene()
{
	for (const auto& obj : m_SceneData.m_Objects)
	{
		SpawnEntity(obj);
	}
}

void UScene::SetupInputActions()
{
	m_InputActions[EInputKey::W] = [this](float deltaTime)
		{
			EInputKeyStatus status = UInputManager::GetInstance().GetKeyStatus(EInputKey::W);
			if (status == EInputKeyStatus::IKS_Pressed)
				m_LocationDebug.Z += 0.1f * deltaTime;
		};
	m_InputActions[EInputKey::S] = [this](float deltaTime)
		{
			EInputKeyStatus status = UInputManager::GetInstance().GetKeyStatus(EInputKey::S);
			if (status == EInputKeyStatus::IKS_Pressed)
				m_LocationDebug.Z -= 0.1f * deltaTime;
		};
}

void UScene::CreateShaders()
{
	// Create default shader
	m_DefaultShaderEntity = m_Application->GetEnttRegistry().create();

	if (m_Application->GetEnttRegistry().valid(m_DefaultShaderEntity))
	{
		m_Application->GetEnttRegistry().emplace<FShaderOpenGLComponent>(m_DefaultShaderEntity, defaultShaderName, "Assets/Shaders/OpenGL/vert.glsl", "Assets/Shaders/OpenGL/frag.glsl");

		FShaderOpenGLComponent& shaderComponent = m_Application->GetEnttRegistry().get<FShaderOpenGLComponent>(m_DefaultShaderEntity);
		shaderComponent.Initialize();

		m_DefaultShader = &shaderComponent;
	}

	// Create Shader Debug
	auto debugShader = m_Application->GetEnttRegistry().create();

	if (m_Application->GetEnttRegistry().valid(debugShader))
	{
		m_Application->GetEnttRegistry().emplace<FShaderOpenGLComponent>(debugShader, "debugShader", "Assets/Shaders/OpenGL/debug_vert.glsl", "Assets/Shaders/OpenGL/debug_frag.glsl");

		FShaderOpenGLComponent& shaderComponent = m_Application->GetEnttRegistry().get<FShaderOpenGLComponent>(debugShader);
		shaderComponent.Initialize();
	}
}

void UScene::SpawnEntity(const TSceneObject& sceneObject)
{
	auto& registry = m_Application->GetEnttRegistry();
	entt::entity entity = registry.create();

	auto& transform = registry.emplace<FTransformComponent>(entity);
	transform.Location = sceneObject.m_Position;
	transform.Rotation = sceneObject.m_Rotation;
	transform.Scale = sceneObject.m_Scale;
	transform.ForwardVector = sceneObject.m_ForwardVector;
	transform.UpVector = sceneObject.m_UpVector;

	auto& identity = registry.emplace<FIdentityComponent>(entity, sceneObject.m_ObjectName);
	identity.SetId(sceneObject.m_Id);
	identity.SetName(sceneObject.m_ObjectName);

}

void UScene::SpawnEntityFromJson(const nlohmann::json& jsonObject)
{
	for (const auto& obj : jsonObject["Objects"])
	{
		auto& registry = m_Application->GetEnttRegistry();
		entt::entity entity = registry.create();

		std::string objectName = obj["ObjectName"].get<std::string>();

		if (objectName == "MainCamera")
			m_MainCameraEntity = entity;

		if (obj.contains("Components")) {
			for (const auto& compJson : obj["Components"]) {
				std::string type = compJson["Type"];
				if (FComponentBuilder::Contains(type))
				{
					FComponentBuilder::Build(type, registry, entity, compJson);
				}
				else {
					std::cerr << "Componente desconhecido: " << type << "\n";
				}
			}
		}
	}

}
