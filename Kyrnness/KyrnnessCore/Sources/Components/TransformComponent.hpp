#pragma once

#ifndef KYRNESS_TRANSFORM_HPP
#define KYRNESS_TRANSFORM_HPP

#include "Components/Component.hpp"
#include "glm/glm.hpp"

class UTransformComponent : public UComponent
{
public:

	glm::vec3 Location{ 0.f };
	glm::vec3 Rotation{ 0.f };
	glm::vec3 Scale{ 1.f };
	glm::vec3 ForwardVector = World::ForwardVector;
	glm::vec3 UpVector = World::UpVector;
	glm::vec3 RightVector = glm::normalize(glm::cross(ForwardVector, UpVector));

	virtual nlohmann::json GetJsonData() override;
};

#endif // KYRNESS_TRANSFORM_HPP