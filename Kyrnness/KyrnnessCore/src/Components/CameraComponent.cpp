#include "pch.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Runtime/Application.hpp"

void FCameraComponent::UpdateAspectRatio(float width, float height)
{
	AspectRatio = width / height;
}

void FCameraComponent::UpdateDirectionFromYawPitch(float yaw, float pitch)
{
	if (m_Application)
	{
		Yaw += yaw;
		Pitch += pitch;

		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;

		FTransformComponent& transform = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);

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

nlohmann::json FCameraComponent::GetJsonData()
{
	nlohmann::json jsonData;

	jsonData["Type"] = "FCameraComponent";
	jsonData["Update"] = bCanUpdate;
	jsonData["Fov"] = Fov;
	jsonData["Near"] = Near;
	jsonData["Far"] = Far;
	jsonData["LocationOffset"] = { LocationOffset.x, LocationOffset.y, LocationOffset.z };

	return jsonData;
}

float FCameraComponent::GetAspectRatio() const
{
	return AspectRatio;
}

FMatrix FCameraComponent::GetProjectionMatrix() const
{
	return FMatrix::Perspective(FMath::Radians(Fov), AspectRatio, Near, Far);
}


//glm::mat4 FCameraComponent::GetViewMatrix() const
//{
//	if (m_Application)
//	{
//		FTransformComponent& transform = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
//
//		return glm::lookAt(
//			transform.Location,
//			transform.Location + transform.ForwardVector,
//			transform.UpVector
//		);
//	}
//
//	return glm::mat4(1.0f); // Return identity matrix if application is not set
//}

FMatrix FCameraComponent::GetViewMatrix() const
{
	if (m_Application)
	{
		FTransformComponent& transform = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);

		FVector location = transform.Location + LocationOffset;

		return FMatrix::LookAt(
			location,
			location + transform.ForwardVector,
			transform.UpVector
		);
	}

	return glm::mat4(1.0f); // Return identity matrix if application is not set
}

void FCameraComponent::Initialize()
{
	FComponent::Initialize();
}

void FCameraComponent::Update(float deltaTime)
{

}
