/*********************************************************************
 *   File: TransformComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef K_TRANSFORM_HPP
#define K_TRANSFORM_HPP

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

#endif // K_TRANSFORM_HPP