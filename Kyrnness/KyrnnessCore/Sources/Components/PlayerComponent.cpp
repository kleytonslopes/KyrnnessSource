/*********************************************************************
 *   File: PlayerComponent.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/CapsuleComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Controller.hpp"
#include "Input/InputManager.hpp"


using InputAction = TFunction<void(int)>;

void UPlayerComponent::OnInitialize()
{
	SetupPlayerInput();

	Super::OnInitialize();
}

void UPlayerComponent::SetupPlayerInput()
{
	if (m_Application)
	{
		m_Application->OnUpdateEvent.AddListener(this, &UPlayerComponent::Update);

		if (UController* controller = m_Application->GetController())
		{
			controller->OnKeyPressedEvent.AddListener(this, &UPlayerComponent::OnKeyPressed);
			controller->OnMouseMoveEvent.AddListener(this, &UPlayerComponent::OnMouseMove);
		}
	}
}

void UPlayerComponent::OnUpdate(float deltaTime)
{
	Super::OnUpdate(deltaTime);

	if (UInputManager::Get().GetInputMode() == EInputMode::EIM_UI)
		return;

	if (bWantMoveForward) MoveForward(deltaTime);
	if (bWantMoveBackward) MoveBackward(deltaTime);
	if (bWantMoveRight) MoveRight(deltaTime);
	if (bWantMoveLeft) MoveLeft(deltaTime);
	if (bWantMoveDown) MoveDown(deltaTime);
	if (bWantMoveUp) MoveUp(deltaTime);
	if (bWantJump) Jump(deltaTime);

	
}

void UPlayerComponent::UpdateVerticalMovement(float deltaTime)
{
	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
		capsuleComp.UpdateVerticalMovement(deltaTime);
	}
}

void UPlayerComponent::OnKeyPressed(int keyCode, int status)
{
	if (m_Application)
	{
		if (m_InputActions.find(keyCode) == m_InputActions.end())
			return;

		m_InputActions[keyCode](status);
	}
}

void UPlayerComponent::OnMouseMove(float x, float y)
{
	if (UInputManager::Get().GetInputMode() == EInputMode::EIM_UI)
		return;

	UCameraComponent& cameraComp = m_Application->GetEnttRegistry().get<UCameraComponent>(m_EntityOwner);
	cameraComp.UpdateDirectionFromYawPitch(x * m_MouseSensitivity, y * m_MouseSensitivity);
}

nlohmann::json UPlayerComponent::GetJsonData()
{
	return nlohmann::json();
}

void UPlayerComponent::MoveForward(float deltaTime)
{
	
	if (!m_Application) return;

	UTransformComponent& comp = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

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

	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
		capsuleComp.Move(direction, deltaTime * m_MoveSpeed);
	}
}

void UPlayerComponent::MoveBackward(float deltaTime)
{
	if (!m_Application) return;

	UTransformComponent& comp = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

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

	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		direction = -direction;

		UCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
		capsuleComp.Move(direction, deltaTime * m_MoveSpeed);
	}
}

void UPlayerComponent::MoveRight(float deltaTime)
{
	if (!m_Application) return;

	UTransformComponent& comp = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

	glm::vec3 direction = comp.RightVector;
	if (!bIsFlyCamera)
	{
		direction.y = 0.0f;
		if (glm::length(direction) > 0.001f)
			direction = glm::normalize(direction);
		else
			direction = glm::vec3(0.0f);
	}
	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
		capsuleComp.Move(direction, deltaTime * m_MoveSpeed);
	}
}

void UPlayerComponent::MoveLeft(float deltaTime)
{
	if (!m_Application) return;

	UTransformComponent& comp = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);

	glm::vec3 direction = -comp.RightVector;
	if (!bIsFlyCamera)
	{
		direction.y = 0.0f;
		if (glm::length(direction) > 0.001f)
			direction = glm::normalize(direction);
		else
			direction = glm::vec3(0.0f);
	}

	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
		capsuleComp.Move(direction, deltaTime * m_MoveSpeed);
	}
}

void UPlayerComponent::MoveDown(float deltaTime)
{
	if (!m_Application) return;

	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UTransformComponent& comp = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);
		comp.Location -= comp.UpVector * deltaTime * m_MoveSpeed;
	}
}

void UPlayerComponent::MoveUp(float deltaTime)
{
	if (!m_Application) return;

	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UTransformComponent& comp = m_Application->GetEnttRegistry().get<UTransformComponent>(m_EntityOwner);
		comp.Location += comp.UpVector * deltaTime * m_MoveSpeed;
	}
}

void UPlayerComponent::Jump(float deltaTime)
{
	if (!m_Application) return;

	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UCapsuleComponent& capsule = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
		capsule.Jump();
	}

	bWantJump = false; // dispara apenas uma vez

	//UCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
	//if(!capsuleComp.GetIsJumping())
	//	capsuleComp.SetIsJumping(true);
}

void UPlayerComponent::Move(FVector Direction, float Scale)
{
	if (m_Application->GetEnttRegistry().any_of<UCapsuleComponent>(m_EntityOwner))
	{
		UCapsuleComponent& capsuleComp = m_Application->GetEnttRegistry().get<UCapsuleComponent>(m_EntityOwner);
		capsuleComp.Move(Direction, Scale * m_MoveSpeed);
	}
}
