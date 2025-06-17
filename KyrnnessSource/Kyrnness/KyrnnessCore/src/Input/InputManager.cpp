#include "pch.hpp"
#include "Input/InputManager.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Controller.hpp"

UInputManager& UInputManager::GetInstance()
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

	//if (status == EInputKeyStatus::IKS_Pressed)
	//	m_Application->GetController()->OnKeyPressed(keyCode);
	//else if (status == EInputKeyStatus::IKS_Released)
	//	m_Application->GetController()->OnKeyReleased(keyCode);
}

void UInputManager::OnMouseMove(float x, float y)
{
	if (m_Application)
	{
		m_Application->GetController()->OnMouseMove(x, y);
	}
}
