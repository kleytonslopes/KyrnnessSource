#pragma once

#ifndef KYRNNES_CORE_JSON_HPP
#define KYRNNES_CORE_JSON_HPP

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include "GameFramework/Scene.hpp"

class FJson
{
public:

	static inline void FromJson(const nlohmann::json& j, glm::vec3& vec)
	{
		j.at(0).get_to(vec.x);
		j.at(1).get_to(vec.y);
		j.at(2).get_to(vec.z);
	}

	static inline void FromJson(const nlohmann::json& j, std::string& value)
	{
		j.get_to(value);
	}

	static inline void FromJson(const nlohmann::json& j, TSceneObject& sceneObject)
	{
		j.at("Id").get_to(sceneObject.m_Id);
		j.at("ObjectName").get_to(sceneObject.m_ObjectName);

		if (j.contains("Position"))			FromJson(j.at("Position"), sceneObject.m_Position);
		if (j.contains("Rotation"))			FromJson(j.at("Rotation"), sceneObject.m_Rotation);
		if (j.contains("Scale"))			FromJson(j.at("Scale"), sceneObject.m_Scale);
		if (j.contains("UpVector"))			FromJson(j.at("UpVector"), sceneObject.m_UpVector);
		if (j.contains("ForwardVector"))	FromJson(j.at("ForwardVector"), sceneObject.m_ForwardVector);

	}

	static inline void FromJson(const nlohmann::json& j, TScene& scene)
	{
		j.at("SceneName").get_to(scene.m_SceneName);
		scene.m_Objects.reserve(j.at("Objects").size());

		for (const auto& obj : j.at("Objects"))
		{
			TSceneObject sceneObject;
			FromJson(obj, sceneObject);
			scene.m_Objects.emplace_back(&sceneObject);
		}
	}

};

#endif // KYRNNES_CORE_JSON_HPP
