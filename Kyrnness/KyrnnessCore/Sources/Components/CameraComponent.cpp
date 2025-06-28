/*********************************************************************
 *   File: CapsuleComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Runtime/Application.hpp"

void UCameraComponent::UpdateAspectRatio(float width, float height)
{
	AspectRatio = width / height;
}

void UCameraComponent::UpdateDirectionFromYawPitch(float yaw, float pitch)
{
	if (m_Application)
	{
		Yaw += yaw;
		Pitch += pitch;

		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;

		UTransformComponent& transform = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

		glm::vec3 direction;
		direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		direction.y = sin(glm::radians(Pitch));
		direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		transform.ForwardVector = glm::normalize(direction);

		// Usualmente o mundo usa Y-up
		transform.RightVector = glm::normalize(glm::cross(transform.ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
		transform.UpVector = glm::normalize(glm::cross(transform.RightVector, transform.ForwardVector));
	}
}

nlohmann::json UCameraComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "UCameraComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["Fov"] = Fov;
	jsonData["Near"] = Near;
	jsonData["Far"] = Far;
	jsonData["LocationOffset"] = { LocationOffset.x, LocationOffset.y, LocationOffset.z };

	return jsonData;
}

float UCameraComponent::GetAspectRatio() const
{
	return AspectRatio;
}

FMatrix UCameraComponent::GetProjectionMatrix() const
{
	return FMatrix::Perspective(FMath::Radians(Fov), AspectRatio, Near, Far);
}

FMatrix UCameraComponent::GetViewMatrix() const
{
	if (m_Application)
	{
		UTransformComponent& transform = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

		FVector location = transform.Location + LocationOffset;

		return FMatrix::LookAt(
			location,
			location + transform.ForwardVector,
			transform.UpVector
		);
	}

	return glm::mat4(1.0f); // Return identity matrix if application is not set
}
