#pragma once

#ifndef KYRNNESS_SCENE_HPP
#define KYRNNESS_SCENE_HPP

#include "Core/Core.hpp"
#include "nlohmann/json.hpp"

class UApplication;
class FShaderOpenGLComponent;

struct TSceneObject
{
	std::string m_ObjectName;
	std::string m_Id;
	
	FVector m_Position      = FVector(0.0f);
	FVector m_Rotation      = FVector(0.0f);
	FVector m_Scale         = FVector(1.0f);
	FVector m_UpVector      = FVector(0.0f, 0.0f, 1.0f);
	FVector m_ForwardVector = FVector(1.0f, 0.0f, 0.0f);

	std::vector<std::string> m_Components;
};

struct TScene
{
	std::string m_SceneName;

	std::vector<TSceneObject> m_Objects;
};



class UScene
{
	const std::string defaultShaderName = "DefaultShader";
	using InputAction = std::function<void(float)>;

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

	float GetGravity() const { return m_Gravity; }

private:
	UApplication* m_Application = nullptr;
	TScene m_SceneData;

	std::unordered_map<int32, InputAction> m_InputActions;

	entt::entity m_MainCameraEntity = entt::null;
	entt::entity m_DefaultShaderEntity = entt::null;

	FVector m_LocationDebug = FVector(0.0f);

	float m_Gravity = -20.8f;

	FShaderOpenGLComponent* m_DefaultShader = nullptr;

	void MakeScene();

	void SetupInputActions();

	//void CreateMainCamera();
	void CreateShaders();

	//void UpdateSceneObjectsAfterLoaded();

	void SpawnEntity(const TSceneObject& sceneObject);
	void SpawnEntityFromJson(const nlohmann::json& jsonObject);
};

#endif // KYRNNESS_SCENE_HPP

