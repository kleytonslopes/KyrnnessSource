#include "pch.hpp"
#include "GameFramework/Controller.hpp"
#include "Runtime/Application.hpp"
#include "Components/CapsuleComponent.hpp"

UController::UController(UApplication* application) : m_Application(application)
{
}

void UController::Initialize()
{
	if (m_Application)
	{
		auto view = m_Application->GetEnttRegistry().view<FCapsuleComponent>();

		view.each([this](const auto entity, auto& capsule)
			{
				m_Pawn = entity;
			});
	}
}

void UController::Update(float DeltaTime)
{

}

void UController::Shutdown()
{

}

void UController::OnKeyPressed(int Key)
{
	OnKeyPressedEvent.Broadcast(Key, 1);
}

void UController::OnKeyReleased(int Key)
{
	OnKeyPressedEvent.Broadcast(Key, 0);
}

void UController::OnMouseMove(float x, float y)
{
	OnMouseMoveEvent.Broadcast(x, y);
}
