/*********************************************************************
 *   File: Scene.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef K_SCENE_HPP
#define K_SCENE_HPP

#include "Class.hpp"
#include "nlohmann/json.hpp"
#include "Managements/EntityManagement.hpp"

class UApplication;
class UShaderOpenGLComponent;
class UCameraComponent;
class UComponent;
class UScene;

struct TSceneObject
{
public:
	TSceneObject(){}
	
	entt::entity m_Entity = entt::null;

	std::string m_ObjectName;
	std::string m_Id;
	
	glm::vec3 m_Position      = glm::vec3(0.0f);
	glm::vec3 m_Rotation      = glm::vec3(0.0f);
	glm::vec3 m_Scale         = glm::vec3(1.0f);
	glm::vec3 m_UpVector      = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_ForwardVector = glm::vec3(1.0f, 0.0f, 0.0f);

	bool bIsSelected = false;

	bool bIsSelectable = true;

	//TMap<std::string, UComponent*> m_Components;

	void SetScene(UScene& sceneRef) { m_Scene = &sceneRef; }
	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);

	UScene& GetScene() { return *m_Scene; }

private:
	UScene* m_Scene;
};

struct TScene
{
	std::string m_SceneName;

	std::vector<TSceneObject*> m_Objects;
};



class UScene : public UClass
{
	using Super = UClass;
	const std::string defaultShaderName = "DefaultShader";
	using InputAction = TFunction<void(float)>;

public:
	UScene(UApplication* application);
	virtual ~UScene();

	UScene(const UScene&) = delete;
	UScene& operator=(const UScene&) = delete;
	UScene(UScene&&) = delete;
	UScene& operator=(UScene&&) = delete;

	void Initialize() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	void DrawScene(float deltaTime);
	void SaveScene();
	void LoadFromFile(const std::string& sceneFile);

	float GetGravity() const { return m_Gravity; }

	TSceneObject* GetSelectedObject(float mouseX, float mouseY, glm::mat4 projection, glm::mat4 view);
	TSceneObject* GetSelectedObject() { return m_SelectedObject; }
	TSceneObject* DuplicateObject(TSceneObject* object);

	glm::vec3 ScreenPosToWorldRay(float mouseX, float mouseY, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	std::vector<TSceneObject*>& GetObjects();
	void ProcessEditMode();
	void SpawnEntityFromJson(const nlohmann::json& jsonObject);

	UEntityManagement* GetEntityManager() { return m_EntityManagement; }

protected:
	void OnUpdate(float DeltaTime) override;
	void PreInitialize() override;

private:
	UApplication* m_Application = nullptr;
	TScene m_SceneData;
	std::string m_SceneFile;
	TMap<int32, InputAction> m_InputActions;

	//entt::entity m_MainCameraEntity = entt::null;
	//entt::entity m_DefaultShaderEntity = entt::null;

	FVector m_LocationDebug = FVector(0.0f);

	float m_Gravity = -20.8f;

	///UShaderOpenGLComponent* m_DefaultShader = nullptr;
	///UCameraComponent* m_MainCamera = nullptr;
	TSceneObject* m_SelectedObject = nullptr;

	UEntityManagement* m_EntityManagement = nullptr;

	void MakeScene();

	void SetupInputActions();

	//void CreateMainCamera();
	void CreateShaders();

	//void UpdateSceneObjectsAfterLoaded();

	void SpawnEntity(const TSceneObject& sceneObject);
	

	friend class UUIEditor;
};

#endif // K_SCENE_HPP

