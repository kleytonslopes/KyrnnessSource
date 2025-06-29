/*********************************************************************
 *   File: CapsuleComponent.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once

#ifndef K_CAMERA_HPP
#define K_CAMERA_HPP

#include "Components/Component.hpp"

struct UTransformComponent;

class UCameraComponent : public UComponent
{
public:
	float Fov = 45.0f;
	float Near = 0.0001f;
	float Far = 100000.f;

	float Yaw = 0.0f;
	float Pitch = 0.0f;

	glm::vec3 LocationOffset{0.f};

	float GetAspectRatio() const;

	FMatrix GetProjectionMatrix() const;
	FMatrix GetViewMatrix() const;

	void UpdateAspectRatio(float width, float height);
	void UpdateDirectionFromYawPitch(float yaw, float pitch);

	virtual nlohmann::json GetJsonData() override;

private:
	float AspectRatio = 1.0f;
};

#endif // K_CAMERA_HPP