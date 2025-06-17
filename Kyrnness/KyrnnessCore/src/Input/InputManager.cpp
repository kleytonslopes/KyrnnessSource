#include "pch.hpp"
#include "Input/InputManager.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Controller.hpp"
#include "Runtime/Window.hpp"

UInputManager& UInputManager::Get()
{
	static UInputManager Instance;
	return Instance;
}

void UInputManager::SetupApplication(UApplication* application)
{
	m_Application = application;
}

EInputKeyStatus UInputManager::GetKeyStatus(int32 keyCode)
{
	for (const auto& [KeyCode, Input] : m_Inputs)
	{
		if (KeyCode == keyCode)
			return Input.Status;
	}

	return IKS_None;
}

void UInputManager::SetKeyPressed(int32 keyCode, EInputKeyStatus status)
{
	
	for (auto& [KeyCode, Input] : m_Inputs)
	{
		if (KeyCode == keyCode)
		{
			if (status == EInputKeyStatus::IKS_Pressed && Input.Status == EInputKeyStatus::IKS_Released)
			{
				Input.Status = status;
				m_Application->GetController()->OnKeyPressed(KeyCode);
			}
			else if (status == EInputKeyStatus::IKS_Released && Input.Status == EInputKeyStatus::IKS_Pressed)
			{
				Input.Status = status;
				m_Application->GetController()->OnKeyReleased(KeyCode);
			}

			return;
		}
	}

	// Se não existe, adiciona
	TInputKey newKey;
	newKey.KeyCode = keyCode;
	newKey.Status = status;
	m_Inputs[keyCode] = newKey;
}

void UInputManager::OnMouseMove(float x, float y)
{
	m_MousePosX = x;
	m_MousePosY = y;

	

	if (m_InputMode != EInputMode::EIM_UI && m_Application)
	{
		m_Application->GetController()->OnMouseMove(x, y);
	}
}

void UInputManager::OnMousePosition(float x, float y)
{
	OnMouseMoveEvent.Broadcast(x, y);
}

void UInputManager::OnMouseButton(float x, float y, bool down, bool up)
{
	m_MousePosX = x;
	m_MousePosY = y;
	m_MouseDown = down;
	m_MouseUp = up;

	OnMouseButtonEvent.Broadcast(x, y, down, up);

}

void UInputManager::GetMousePosition(float& outX, float& outY) const
{
	outX = m_MousePosX;
	outY = m_MousePosY;
}

void UInputManager::GetMouseButtonState(int32 button, EInputKeyStatus& outStatus) const
{
}

void UInputManager::SetShowMouseCursor(bool show)
{
	b_ShowMouseCursor = show;

	if (m_Application)
	{
		m_Application->GetWindow()->SetShowMouseCursor(show);
	}
}

void UInputManager::SetInputMode(EInputMode mode)
{
	if (mode == m_InputMode)
		return;

	m_InputMode = mode;

	if (m_Application && m_InputMode == EInputMode::EIM_UI)
	{
		m_Application->GetWindow()->SetShowMouseCursor(true);
	}
}
