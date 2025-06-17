#include "pch.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/CapsuleComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Controller.hpp"


using InputAction = std::function<void(int)>;

void FPlayerComponent::Initialize()
{
	SetupPlayerInput();
}

void FPlayerComponent::SetupPlayerInput()
{
	if (m_Application)
	{
		m_Application->OnUpdateEvent.AddListener(this, &FPlayerComponent::Update);

		if (UController* controller = m_Application->GetController())
		{
			controller->OnKeyPressedEvent.AddListener(this, &FPlayerComponent::OnKeyPressed);
			controller->OnMouseMoveEvent.AddListener(this, &FPlayerComponent::OnMouseMove);
		}
	}
}

void FPlayerComponent::Update(float deltaTime)
{
	if (bWantMoveForward) MoveForward(deltaTime);
	if (bWantMoveBackward) MoveBackward(deltaTime);
	if (bWantMoveRight) MoveRight(deltaTime);
	if (bWantMoveLeft) MoveLeft(deltaTime);
	if (bWantMoveDown) MoveDown(deltaTime);
	if (bWantMoveUp) MoveUp(deltaTime);
	if (bWantJump) Jump(deltaTime);
}

void FPlayerComponent::UpdateVerticalMovement(float deltaTime)
{
	FCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<FCapsuleComponent>(m_EntityOwner);
	capsuleComp.UpdateVerticalMovement(deltaTime);


}

void FPlayerComponent::OnKeyPressed(int keyCode, int status)
{
	if (m_Application)
	{
		if (m_InputActions.find(keyCode) == m_InputActions.end())
			return;

		m_InputActions[keyCode](status);
	}
}

void FPlayerComponent::OnMouseMove(float x, float y)
{
	FCameraComponent& cameraComp = m_Application->GetEnttRegistry().get<FCameraComponent>(m_EntityOwner);
	cameraComp.UpdateDirectionFromYawPitch(x * m_MouseSensitivity, y * m_MouseSensitivity);
}

void FPlayerComponent::MoveForward(float deltaTime)
{
	if (!m_Application) return;

	FTransformComponent& comp = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);

	glm::vec3 direction = comp.ForwardVector;

	if (!bIsFlyCamera)
	{
		// Projeta o vetor para o plano XZ (anula componente vertical)
		direction.y = 0.0f;

		// Normaliza para evitar velocidade menor quando olhamos diagonalmente
		if (glm::length(direction) > 0.001f)
			direction = glm::normalize(direction);
		else
			direction = glm::vec3(0.0f);
	}

	FCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<FCapsuleComponent>(m_EntityOwner);
	capsuleComp.Move(direction, deltaTime * m_MoveSpeed);
}

void FPlayerComponent::MoveBackward(float deltaTime)
{
	if (!m_Application) return;

	FTransformComponent& comp = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
	
	glm::vec3 direction = comp.ForwardVector;

	if (!bIsFlyCamera)
	{
		// Projeta o vetor para o plano XZ (anula componente vertical)
		direction.y = 0.0f;

		// Normaliza para evitar velocidade menor quando olhamos diagonalmente
		if (glm::length(direction) > 0.001f)
			direction = glm::normalize(direction);
		else
			direction = glm::vec3(0.0f);
	}

	direction = -direction;

	FCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<FCapsuleComponent>(m_EntityOwner);
	capsuleComp.Move(direction, deltaTime * m_MoveSpeed);
}

void FPlayerComponent::MoveRight(float deltaTime)
{
	if (!m_Application) return;

	FTransformComponent& comp = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
	
	glm::vec3 direction = comp.RightVector;
	if (!bIsFlyCamera)
	{
		direction.y = 0.0f;
		if (glm::length(direction) > 0.001f)
			direction = glm::normalize(direction);
		else
			direction = glm::vec3(0.0f);
	}

	FCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<FCapsuleComponent>(m_EntityOwner);
	capsuleComp.Move(direction, deltaTime * m_MoveSpeed);

	/*comp.Location += comp.RightVector * deltaTime * m_MoveSpeed;*/
}

void FPlayerComponent::MoveLeft(float deltaTime)
{
	if (!m_Application) return;

	FTransformComponent& comp = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
	
	FVector direction = -comp.RightVector;
	if (!bIsFlyCamera)
	{
		direction.Y = 0.0f;
		if (direction.Length() > 0.001f)
			direction = FVector::Normalize(direction);
		else
			direction = FVector(0.0f);
	}

	FCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<FCapsuleComponent>(m_EntityOwner);
	capsuleComp.Move(direction, deltaTime * m_MoveSpeed);
}

void FPlayerComponent::MoveDown(float deltaTime)
{
	if (!m_Application) return;

	FTransformComponent& comp = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
	comp.Location -= comp.UpVector * deltaTime * m_MoveSpeed;
}

void FPlayerComponent::MoveUp(float deltaTime)
{
	if (!m_Application) return;

	FTransformComponent& comp = m_Application->GetEnttRegistry().get<FTransformComponent>(m_EntityOwner);
	comp.Location += comp.UpVector * deltaTime * m_MoveSpeed;
}

void FPlayerComponent::Jump(float deltaTime)
{
	if (!m_Application) return;

	FCapsuleComponent& capsule = m_Application->GetEnttRegistry().get<FCapsuleComponent>(m_EntityOwner);
	capsule.Jump();

	bWantJump = false; // dispara apenas uma vez

	//FCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<FCapsuleComponent>(m_EntityOwner);
	//if(!capsuleComp.GetIsJumping())
	//	capsuleComp.SetIsJumping(true);
}
