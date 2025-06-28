/*********************************************************************
 *   File: Scene.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
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
	Super::Initialize();

	m_EntityManagement = FMemoryManager::Allocate<UEntityManagement>();
	m_EntityManagement->Initialize();

	SetupInputActions();
}

void UScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	for (auto& [key, action] : m_InputActions)
	{
		action(deltaTime);
	}
}

void UScene::OnDestroy()
{
	Super::OnDestroy();

	//auto& registry = m_Application->GetRegistry();

	///// 1. Limpar os SceneObjects
	///for (TSceneObject* obj : m_SceneData.m_Objects)
	///{
	///	if (obj)
	///	{
	///		// Apenas limpar o mapa de componentes (não precisa deletar os componentes, pois o entt fará isso)
	///		obj->m_Components.clear();
	///
	///		// Liberar o próprio TSceneObject
	///		FMemoryManager::Deallocate(obj);
	///	}
	///}

	m_SceneData.m_Objects.clear();

	// 2. Resetar ponteiros da cena
	//m_MainCamera = nullptr;
	//m_DefaultShader = nullptr;
	//m_MainCameraEntity = entt::null;

	// 3. Limpar o registry (isso deleta os componentes internamente)
	//registry.clear();

	m_EntityManagement->Destroy();

	FLogger::Information("[Scene] Cena limpa!\n");
}

void UScene::DrawScene(float deltaTime)
{
	if (!IsValid())
		return;

	auto mainCameraView = m_EntityManagement->GetRegistry().view<UMainCamera>();
	FEntity mainCameraActive = entt::null;

	mainCameraView.each([&](const auto& entity, UMainCamera& cameraComp)
		{
			if (mainCameraActive == entt::null)
				mainCameraActive = entity;

			if(cameraComp.GetIsActive())
				mainCameraActive = entity;

		});

	if (m_EntityManagement->IsValidEntity(mainCameraActive))
	{
		auto objectsSceneView = m_EntityManagement->GetRegistry().view<UTransformComponent, UMeshRenderer_OpenGLComponent>();

		UCameraComponent& cameraScene = m_EntityManagement->GetRegistry().get<UCameraComponent>(mainCameraActive);

		cameraScene.UpdateAspectRatio(m_Application->GetWidth(), m_Application->GetHeight());

		objectsSceneView.each([&](const auto entity, auto& transform, auto& meshRenderer)
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
	FLogger::Error("Save Scene not Completed, Check this method!");
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

		//for (const auto& [key, component] : obj->m_Components)
		//{
		//	if (component)
		//	{
		//		const auto& a = component->GetJsonData();
		//
		//		objJson["Components"].push_back(a);
		//	}
		//}

		sceneJson["Objects"].push_back(objJson);
	}

	UAssetManager::SaveJson("Content/Maps/EntryMap.json", sceneJson);
}

void UScene::LoadFromFile(const std::string& sceneFile)
{
	m_SceneFile = sceneFile;

}

TSceneObject* UScene::GetSelectedObject(float mouseX, float mouseY, glm::mat4 projection, glm::mat4 view)
{
	float screenWidth = m_Application->GetWidth();
	float screenHeight = m_Application->GetHeight();

	glm::vec3 ray = ScreenPosToWorldRay(mouseX, mouseY, projection, view);
	glm::vec3 rayOrigin = glm::vec3(glm::inverse(view)[3]); // Posi��o da c�mera

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
		if (m_Application->GetWindow()->GetMouseButtonState(1) == 1)
		{
			float x, y;
			m_Application->GetWindow()->GetMousePosition(x, y);

			// Desselecionar tudo primeiro
			for (auto& obj : m_SceneData.m_Objects) {
				obj->bIsSelected = false;
			}

			// Tentar selecionar objeto
	/*		TSceneObject* selected = GetSelectedObject(
				(float)x, (float)y,
				m_MainCamera->GetProjectionMatrix(),
				m_MainCamera->GetViewMatrix());*/

				//if (selected) {
				//	selected->bIsSelected = true;
				//	m_SelectedObject = selected;
				//}
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

//void UScene::CreateMainCamera()
//{
//	m_MainCameraEntity = m_EntityManagement->CreateEntity();
//
//	if (m_Application->GetRegistry().valid(m_MainCameraEntity))
//	{
//		m_Application->GetRegistry().emplace<UTransformComponent>(m_MainCameraEntity);
//		m_Application->GetRegistry().emplace<UCameraComponent>(m_MainCameraEntity);
//		m_Application->GetRegistry().emplace<UIdentityComponent>(m_MainCameraEntity, "MainCamera");
//
//		auto view = m_Application->GetRegistry().view<UTransformComponent, UCameraComponent>();
//
//		view.each([this](const auto entity, auto& transform, auto& camera)
//			{
//				transform.Location = glm::vec3(0.f, 0.f, 5.f);
//
//				camera.Fov = 45.f;
//				camera.Near = 0.0001f;
//				camera.Far = 100000.f;
//				camera.UpdateAspectRatio(m_Application->GetWidth(), m_Application->GetHeight());
//			});
//	}
//}

void UScene::CreateShaders()
{
	// Create default shader
	auto m_DefaultShaderEntity = m_EntityManagement->CreateEntity();

	if (m_EntityManagement->IsValidEntity(m_DefaultShaderEntity))
	{
		UShaderOpenGLComponent& shaderComponent = m_EntityManagement->AddComponent<UShaderOpenGLComponent>(m_DefaultShaderEntity);//m_Application->GetRegistry().get<UShaderOpenGLComponent>(m_DefaultShaderEntity);
		shaderComponent.SetShaderName(defaultShaderName);
		shaderComponent.SetVertShaderFile("Assets/Shaders/OpenGL/vert.glsl");
		shaderComponent.SetFragShaderFile("Assets/Shaders/OpenGL/frag.glsl");

		shaderComponent.Initialize();

		//m_DefaultShader = &shaderComponent;
	}

	// Create Shader Debug
	auto debugShader = m_EntityManagement->CreateEntity();

	if (m_EntityManagement->IsValidEntity(debugShader))
	{
		UShaderOpenGLComponent& shaderComponent = m_EntityManagement->AddComponent<UShaderOpenGLComponent>(debugShader);
		shaderComponent.SetShaderName("debugShader");
		shaderComponent.SetVertShaderFile("Assets/Shaders/OpenGL/debug_vert.glsl");
		shaderComponent.SetFragShaderFile("Assets/Shaders/OpenGL/debug_frag.glsl");

		shaderComponent.Initialize();
	}

	// Create Shader UI
	auto uiShader = m_EntityManagement->CreateEntity();

	if (m_EntityManagement->IsValidEntity(uiShader))
	{
		UShaderOpenGLComponent& shaderComponent = m_EntityManagement->AddComponent<UShaderOpenGLComponent>(uiShader);
		shaderComponent.SetShaderName("uiShader");
		shaderComponent.SetVertShaderFile("uiShader.vert");
		shaderComponent.SetFragShaderFile("uiShader.frag");

		shaderComponent.Initialize();
	}
}

void UScene::SpawnEntity(const TSceneObject& sceneObject)
{
	FEntity entity = m_EntityManagement->CreateEntity();

	UTransformComponent& transform = m_EntityManagement->AddComponent<UTransformComponent>(entity);

	transform.Location = sceneObject.m_Position;
	transform.Rotation = sceneObject.m_Rotation;
	transform.Scale = sceneObject.m_Scale;
	transform.ForwardVector = sceneObject.m_ForwardVector;
	transform.UpVector = sceneObject.m_UpVector;

	UIdentityComponent& identity = m_EntityManagement->AddComponent<UIdentityComponent>(entity, sceneObject.m_ObjectName);
	identity.SetId(sceneObject.m_Id);
	identity.SetName(sceneObject.m_ObjectName);

}

void UScene::SpawnEntityFromJson(const nlohmann::json& jsonObject)
{
	for (const auto& obj : jsonObject["Objects"])
	{
		FEntity entity = m_EntityManagement->CreateEntity();

		TSceneObject* sceneObject = FMemoryManager::Allocate<TSceneObject>();

		sceneObject->m_Entity = entity;

		std::string objectName = obj["ObjectName"].get<std::string>();
		bool isSelectable = obj["IsSelectable"].get<bool>();

		//if (objectName == "MainCamera")
		//	m_MainCameraEntity = entity;

		if (obj.contains("Components")) {
			for (const auto& compJson : obj["Components"]) {
				std::string type = compJson["Type"];
				if (UComponentBuilder::Contains(type))
				{
					UComponentBuilder::Build(type, m_EntityManagement->GetRegistry(), entity, compJson, sceneObject);
				}
				else {
					std::cerr << "Componente desconhecido: " << type << "\n";
				}
			}
		}

		UTransformComponent& transform = m_EntityManagement->GetComponent<UTransformComponent>(entity);//registry.get<UTransformComponent>(entity);
		sceneObject->m_Position = transform.Location;
		sceneObject->m_Rotation = transform.Rotation;
		sceneObject->m_ForwardVector = transform.ForwardVector;
		sceneObject->m_UpVector = transform.UpVector;
		sceneObject->m_Scale = transform.Scale;
		sceneObject->m_ObjectName = objectName;
		sceneObject->bIsSelectable = isSelectable;

		//if (objectName == "MainCamera")
		//{
		//	m_MainCamera = &registry.get<UCameraComponent>(m_MainCameraEntity);
		//}

		m_SceneData.m_Objects.emplace_back(sceneObject);
	}

}

void UScene::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	DrawScene(DeltaTime);
}

void UScene::PreInitialize()
{
	Super::PreInitialize();

	m_EntityManagement = FMemoryManager::Allocate<UEntityManagement>();
}

void TSceneObject::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	if (m_Entity != entt::null)
	{
		auto& transform = m_Scene->GetEntityManager()->GetComponent<UTransformComponent>(m_Entity);
		transform.Location = position;
	}
}

void TSceneObject::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = rotation;
	if (m_Entity != entt::null)
	{
		auto& transform = m_Scene->GetEntityManager()->GetComponent<UTransformComponent>(m_Entity); // UApplication::Get().GetRegistry().get<UTransformComponent>(m_Entity);
		transform.Rotation = rotation;
	}
}
