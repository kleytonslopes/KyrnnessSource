#include "GamePCH.hpp"
#include "Components/MyPlayerComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Runtime/Application.hpp"
#include "GameFramework/Controller.hpp"
#include "Input/InputManager.hpp"


using InputAction = TFunction<void(int)>;

void FMyPlayerComponent::Initialize()
{
	UPlayerComponent::Initialize();

	UInputManager::Get().SetInputMode(EInputMode::EIM_UI);
	UInputManager::Get().SetShowMouseCursor(true);
}

void FMyPlayerComponent::SetupPlayerInput()
{
	UPlayerComponent::SetupPlayerInput();

	if (m_Application)
	{
		m_InputActions[EInputKey::W] = [this](int status) { if (status == 1) bWantMoveForward = true; else bWantMoveForward = false;	};
		m_InputActions[EInputKey::S] = [this](int status) { if (status == 1) bWantMoveBackward = true; else bWantMoveBackward = false;	};
		m_InputActions[EInputKey::A] = [this](int status) { if (status == 1) bWantMoveLeft = true; else bWantMoveLeft = false;	};
		m_InputActions[EInputKey::D] = [this](int status) { if (status == 1) bWantMoveRight = true; else bWantMoveRight = false;	};
		m_InputActions[EInputKey::C] = [this](int status) { if (status == 1) bWantMoveDown = true; else bWantMoveDown = false;	};
		m_InputActions[EInputKey::Space] = [this](int status) { if (status == 1) bWantJump = true; };
		m_InputActions[EInputKey::AltLeft] = [this](int status)
			{ 
				if (status == 1)
				{
					bool currentShowMouse = !UInputManager::Get().GetShowMouseCursor();

					if(currentShowMouse)
						UInputManager::Get().SetInputMode(EInputMode::EIM_UI);
					else
						UInputManager::Get().SetInputMode(EInputMode::EIM_GameMode);

					UInputManager::Get().SetShowMouseCursor(currentShowMouse);
				}
			};
			
	}
}

void FMyPlayerComponent::Update(float deltaTime)
{
	UPlayerComponent::Update(deltaTime);

}

nlohmann::json FMyPlayerComponent::GetJsonData()
{
	nlohmann::json jsonData = UPlayerComponent::GetJsonData();

	jsonData["Type"] = "FMyPlayerComponent";

	return jsonData;
}
