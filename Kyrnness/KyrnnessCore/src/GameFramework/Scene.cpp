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
#include "Graphics/Shaders.hpp"
#include "Runtime/WindowSDL.hpp"

#include <glm/gtx/intersect.hpp>
#include <imgui.h>

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
	//CreateShaders();

	TComponentBuilder& comp = FComponentBuilder::GetComponentBuilderParameters();
	//comp.defaultShader = m_DefaultShader;
	comp.defaultShader = UShaders::GetShader(SHADER_DEFAULT);

	FComponentBuilder::RegisterEngineComponents(comp);

	nlohmann::json sceneJson = UAssetManager::LoadJson(m_Application->GetGameConfig().m_MainMenuMap);

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

void UScene::SaveScene()
{
	nlohmann::json sceneJson;
	sceneJson["SceneName"] = m_SceneData.m_SceneName;
	sceneJson["Objects"] = nlohmann::json::array();

	for (const auto& obj : m_SceneData.m_Objects)
	{
		nlohmann::json objJson;
		objJson["Id"] = obj->m_Id;
		objJson["ObjectName"] = obj->m_ObjectName;
		objJson["IsSelectable"] = obj->bIsSelectable;

		objJson["Components"] = nlohmann::json::array();

		for (const auto& [key, component] : obj->m_Components)
		{
			if (component)
			{
				const auto& a = component->GetJsonData();

				objJson["Components"].push_back(a);
			}
		}

		sceneJson["Objects"].push_back(objJson);
	}

	UAssetManager::SaveJson("Content/Maps/EntryMap.json", sceneJson);
}

TSceneObject* UScene::GetSelectedObject(float mouseX, float mouseY, glm::mat4 projection, glm::mat4 view)
{
	float screenWidth = m_Application->GetWidth();
	float screenHeight = m_Application->GetHeight();

	glm::vec3 ray = ScreenPosToWorldRay(mouseX, mouseY, projection, view);
	glm::vec3 rayOrigin = glm::vec3(glm::inverse(view)[3]); // Posição da câmera

	TSceneObject* closestObject = nullptr;
	float closestDistance = FLT_MAX;

	for (auto& obj : m_SceneData.m_Objects) {
		// Criar bounding box simples (esfera para exemplo)
		float radius = std::max({ obj->m_Scale.x, obj->m_Scale.y, obj->m_Scale.z }) * 0.5f;

		float intersectionDistance;
		if (glm::intersectRaySphere(
			rayOrigin, ray,
			obj->m_Position,
			radius * radius,
			intersectionDistance))
		{
			if (obj->bIsSelectable && intersectionDistance < closestDistance) {
				closestDistance = intersectionDistance;
				closestObject = obj;
			}
		}
	}

	return closestObject;
}

TSceneObject* UScene::DuplicateObject(TSceneObject* object)
{
	TSceneObject* sceneObject = FMemoryManager::Allocate<TSceneObject>();

	*sceneObject = *object;

	sceneObject->m_ObjectName = FGuid::NewGuid().ToString();

	m_SceneData.m_Objects.emplace_back(sceneObject);

	return sceneObject;
}

glm::vec3 UScene::ScreenPosToWorldRay(float mouseX, float mouseY, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	float screenWidth = m_Application->GetWidth();
	float screenHeight = m_Application->GetHeight();

	float x = (2.0f * mouseX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / screenHeight;

	glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

	// Para coordenadas do olho
	glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// Para coordenadas do mundo
	glm::vec3 rayWorld = glm::vec3(glm::inverse(viewMatrix) * rayEye);
	return glm::normalize(rayWorld);
}

std::vector<TSceneObject*>& UScene::GetObjects()
{
	return m_SceneData.m_Objects;
}

void UScene::ProcessEditMode()
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		// Verificar clique do mouse
		if(m_Application->GetWindow()->GetMouseButtonState(1) == 1)
		{
			float x, y;
			m_Application->GetWindow()->GetMousePosition(x, y);

			// Desselecionar tudo primeiro
			for (auto& obj : m_SceneData.m_Objects) {
				obj->bIsSelected = false;
			}

			// Tentar selecionar objeto
			TSceneObject* selected = GetSelectedObject(
				(float)x, (float)y,
				m_MainCamera->GetProjectionMatrix(),
				m_MainCamera->GetViewMatrix());

			if (selected) {
				selected->bIsSelected = true;
				m_SelectedObject = selected;
			}
		}
	}
}

void UScene::MakeScene()
{
	for (const auto& obj : m_SceneData.m_Objects)
	{
		SpawnEntity(*obj);
	}
}

void UScene::SetupInputActions()
{
	m_InputActions[EInputKey::W] = [this](float deltaTime)
		{
			EInputKeyStatus status = UInputManager::Get().GetKeyStatus(EInputKey::W);
			if (status == EInputKeyStatus::IKS_Pressed)
				m_LocationDebug.Z += 0.1f * deltaTime;
		};
	m_InputActions[EInputKey::S] = [this](float deltaTime)
		{
			EInputKeyStatus status = UInputManager::Get().GetKeyStatus(EInputKey::S);
			if (status == EInputKeyStatus::IKS_Pressed)
				m_LocationDebug.Z -= 0.1f * deltaTime;
		};
}

void UScene::CreateMainCamera()
{
	m_MainCameraEntity = m_Application->GetEnttRegistry().create();

	if (m_Application->GetEnttRegistry().valid(m_MainCameraEntity))
	{
		m_Application->GetEnttRegistry().emplace<FTransformComponent>(m_MainCameraEntity);
		m_Application->GetEnttRegistry().emplace<FCameraComponent>(m_MainCameraEntity);
		m_Application->GetEnttRegistry().emplace<FIdentityComponent>(m_MainCameraEntity, "MainCamera");

		auto view = m_Application->GetEnttRegistry().view<FTransformComponent, FCameraComponent>();

		view.each([this](const auto entity, auto& transform, auto& camera)
			{
				transform.Location = glm::vec3(0.f, 0.f, 5.f);

				camera.Fov = 45.f;
				camera.Near = 0.0001f;
				camera.Far = 100000.f;
				camera.UpdateAspectRatio(m_Application->GetWidth(), m_Application->GetHeight());
			});
	}
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

	// Create Shader UI
	auto uiShader = m_Application->GetEnttRegistry().create();

	if (m_Application->GetEnttRegistry().valid(uiShader))
	{
		m_Application->GetEnttRegistry().emplace<FShaderOpenGLComponent>(uiShader, "uiShader", "uiShader.vert", "uiShader.frag");

		FShaderOpenGLComponent& shaderComponent = m_Application->GetEnttRegistry().get<FShaderOpenGLComponent>(uiShader);
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

		TSceneObject* sceneObject = FMemoryManager::Allocate<TSceneObject>();

		sceneObject->m_Entity = entity;

		std::string objectName = obj["ObjectName"].get<std::string>();
		bool isSelectable = obj["IsSelectable"].get<bool>();

		if (objectName == "MainCamera")
			m_MainCameraEntity = entity;

		if (obj.contains("Components")) {
			for (const auto& compJson : obj["Components"]) {
				std::string type = compJson["Type"];
				if (FComponentBuilder::Contains(type))
				{
					FComponentBuilder::Build(type, registry, entity, compJson, sceneObject);
				}
				else {
					std::cerr << "Componente desconhecido: " << type << "\n";
				}
			}
		}

		FTransformComponent& transform = registry.get<FTransformComponent>(entity);
		sceneObject->m_Position = transform.Location;
		sceneObject->m_Rotation = transform.Rotation;
		sceneObject->m_ForwardVector = transform.ForwardVector;
		sceneObject->m_UpVector = transform.UpVector;
		sceneObject->m_Scale = transform.Scale;
		sceneObject->m_ObjectName = objectName;
		sceneObject->bIsSelectable = isSelectable;

		if (objectName == "MainCamera")
		{
			m_MainCamera = &registry.get<FCameraComponent>(m_MainCameraEntity);
		}

		m_SceneData.m_Objects.emplace_back(sceneObject);
	}

}

void TSceneObject::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	if (m_Entity != entt::null)
	{
		auto& transform = UApplication::Get().GetEnttRegistry().get<FTransformComponent>(m_Entity);
		transform.Location = position;
	}
}

void TSceneObject::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = rotation;
	if (m_Entity != entt::null)
	{
		auto& transform = UApplication::Get().GetEnttRegistry().get<FTransformComponent>(m_Entity);
		transform.Rotation = rotation;
	}
}
