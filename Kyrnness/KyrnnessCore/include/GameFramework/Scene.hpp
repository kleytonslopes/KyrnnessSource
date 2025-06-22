#pragma once

#ifndef KYRNNESS_SCENE_HPP
#define KYRNNESS_SCENE_HPP

#include "Core/Core.hpp"
#include "nlohmann/json.hpp"

class UApplication;
class UShaderOpenGLComponent;
class UCameraComponent;
class UComponent;

struct TSceneObject
{
public:
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

	TMap<std::string, UComponent*> m_Components;

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
};

struct TScene
{
	std::string m_SceneName;

	std::vector<TSceneObject*> m_Objects;
};



class UScene
{
	const std::string defaultShaderName = "DefaultShader";
	using InputAction = TFunction<void(float)>;

public:
	UScene(UApplication* application);
	virtual ~UScene();

	UScene(const UScene&) = delete;
	UScene& operator=(const UScene&) = delete;
	UScene(UScene&&) = delete;
	UScene& operator=(UScene&&) = delete;

	void Initialize();
	void Update(float deltaTime);
	void DrawScene(float deltaTime);
	void SaveScene();

	float GetGravity() const { return m_Gravity; }

	TSceneObject* GetSelectedObject(float mouseX, float mouseY, glm::mat4 projection, glm::mat4 view);
	TSceneObject* GetSelectedObject() { return m_SelectedObject; }
	TSceneObject* DuplicateObject(TSceneObject* object);

	glm::vec3 ScreenPosToWorldRay(float mouseX, float mouseY, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	std::vector<TSceneObject*>& GetObjects();
	void ProcessEditMode();
private:
	UApplication* m_Application = nullptr;
	TScene m_SceneData;

	TMap<int32, InputAction> m_InputActions;

	entt::entity m_MainCameraEntity = entt::null;
	entt::entity m_DefaultShaderEntity = entt::null;

	FVector m_LocationDebug = FVector(0.0f);

	float m_Gravity = -20.8f;

	UShaderOpenGLComponent* m_DefaultShader = nullptr;
	UCameraComponent* m_MainCamera = nullptr;
	TSceneObject* m_SelectedObject = nullptr;

	void MakeScene();

	void SetupInputActions();

	void CreateMainCamera();
	void CreateShaders();

	//void UpdateSceneObjectsAfterLoaded();

	void SpawnEntity(const TSceneObject& sceneObject);
	void SpawnEntityFromJson(const nlohmann::json& jsonObject);

	friend class UUIEditor;
};

#endif // KYRNNESS_SCENE_HPP

