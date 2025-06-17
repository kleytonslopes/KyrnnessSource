#pragma once
#ifndef KYRNNESS_INPUT_MANAGER_HPP
#define KYRNNESS_INPUT_MANAGER_HPP

#include "Input/InputKey.hpp"
#include <unordered_map>

class UApplication;

enum class EInputMode
{
	EIM_UI,
	EIM_GameMode,
	EIM_GameModeUI,
};

class UInputManager
{
public:
	//                 x,     y, down,   up
	FFunctionEvent<float, float, bool, bool> OnMouseButtonEvent;
	FFunctionEvent<float, float> OnMouseMoveEvent;

	static UInputManager& Get();

	void SetupApplication(UApplication* application);

	EInputKeyStatus GetKeyStatus(int32 keyCode);
	EInputMode GetInputMode() const { return m_InputMode; }

	void SetKeyPressed(int32 keyCode, EInputKeyStatus status);
	void OnMouseMove(float x, float y);
	void OnMousePosition(float x, float y);
	void OnMouseButton(float x, float y, bool down, bool up);

	void GetMousePosition(float& outX, float& outY) const;
	void GetMouseButtonState(int32 button, EInputKeyStatus& outStatus) const;
	void SetShowMouseCursor(bool show);
	bool GetShowMouseCursor() const { return b_ShowMouseCursor; };
	void SetInputMode(EInputMode mode);

private:
	float m_MousePosX = 0.f;
	float m_MousePosY = 0.f;
	bool m_MouseDown = false;
	bool m_MouseUp = false;

	std::unordered_map<uint32, TInputKey> m_Inputs;

	bool b_ShowMouseCursor = true;

	UApplication* m_Application = nullptr;

	EInputMode m_InputMode = EInputMode::EIM_UI;

	// Construtor privado
	UInputManager() = default;
	~UInputManager() = default;

	UInputManager(const UInputManager&) = delete;
	UInputManager& operator=(const UInputManager&) = delete;
	UInputManager(UInputManager&&) = delete;
	UInputManager& operator=(UInputManager&&) = delete;

};

#endif// KYRNNESS_INPUT_MANAGER_HPP

