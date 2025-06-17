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

		FVector direction;
		direction.X = FMath::Cos(FMath::Radians(Yaw)) * FMath::Cos(FMath::Radians(Pitch));
		direction.Y = FMath::Sin(FMath::Radians(Pitch));
		direction.Z = FMath::Sin(FMath::Radians(Yaw)) * FMath::Cos(FMath::Radians(Pitch));
		transform.ForwardVector = FVector::Normalize(direction);

		// Usualmente o mundo usa Y-up
		transform.RightVector = FVector::Normalize(FVector::Cross(transform.ForwardVector, FVector(0.0f, 1.0f, 0.0f)));//glm::normalize(glm::cross(transform.ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
		transform.UpVector = FVector::Normalize(FVector::Cross(transform.RightVector, transform.ForwardVector));
	}
}

float FCameraComponent::GetAspectRatio() const
{
	return AspectRatio;
}

glm::mat4 FCameraComponent::GetProjectionMatrix() const
{
	glm::mat4 gm = glm::perspective(glm::radians(Fov), AspectRatio, Near, Far);
	return gm;//FMatrix::Perspective(glm::radians(Fov), AspectRatio, Near, Far);
}

glm::mat4 FCameraComponent::GetViewMatrix() const
{
	if (m_Application)
	{
		FTransformComponent& transform = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
		
		FVector location = transform.Location + LocationOffset;

		return FMatrix::LookAt(location, location + transform.ForwardVector, transform.UpVector);
	}

	return FMatrix(1.0f); // Return identity matrix if application is not set
}

void FCameraComponent::Initialize()
{
	FComponent::Initialize();
}

void FCameraComponent::Update(float deltaTime)
{

}
