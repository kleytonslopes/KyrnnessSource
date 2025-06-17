#pragma once
#ifndef KYRNNESS_INPUT_MANAGER_HPP
#define KYRNNESS_INPUT_MANAGER_HPP

#include "Input/InputKey.hpp"
#include <unordered_map>

class UApplication;

class UInputManager
{
public:
	static UInputManager& GetInstance();

	void SetupApplication(UApplication* application);

	EInputKeyStatus GetKeyStatus(int32 keyCode);
	void SetKeyPressed(int32 keyCode, EInputKeyStatus status);
	void OnMouseMove(float x, float y);

private:
	std::unordered_map<uint32, TInputKey> m_Inputs;

	UApplication* m_Application = nullptr;

	// Construtor privado
	UInputManager() = default;
	~UInputManager() = default;

	UInputManager(const UInputManager&) = delete;
	UInputManager& operator=(const UInputManager&) = delete;
	UInputManager(UInputManager&&) = delete;
	UInputManager& operator=(UInputManager&&) = delete;

};

#endif// KYRNNESS_INPUT_MANAGER_HPP

