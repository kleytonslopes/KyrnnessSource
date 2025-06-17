#pragma once
#ifndef KYRNNESS_PLAYER_COMPONENT_HPP
#define KYRNNESS_PLAYER_COMPONENT_HPP

#include "Components/Component.hpp"

class FPlayerComponent : public FComponent
{
	using InputAction = std::function<void(int)>;

public:
	FPlayerComponent() = default;
	virtual ~FPlayerComponent() = default;

	FPlayerComponent(const FPlayerComponent&) = delete;
	FPlayerComponent& operator=(const FPlayerComponent&) = delete;
	FPlayerComponent(FPlayerComponent&&) = delete;
	FPlayerComponent& operator=(FPlayerComponent&&) = delete;

	virtual void Initialize();
	virtual void SetupPlayerInput();
	virtual void Update(float deltaTime);
	void UpdateVerticalMovement(float deltaTime);

	void OnKeyPressed(int keyCode, int status);
	void OnMouseMove(float x, float y);
	
	virtual nlohmann::json GetJsonData() override;

protected:
	std::unordered_map<int32, InputAction> m_InputActions;
	float m_MoveSpeed = 3.0f; 
	float m_MouseSensitivity = 40.f;

	bool bIsFlyCamera = false;

	bool bWantMoveForward = false;
	bool bWantMoveBackward = false;
	bool bWantMoveRight = false;
	bool bWantMoveLeft = false;
	bool bWantMoveDown = false;
	bool bWantMoveUp = false;
	bool bWantJump = false;

	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveDown(float deltaTime);
	void MoveUp(float deltaTime);
	void Jump(float deltaTime);
	void Move(FVector Direction, float Scale);
};

#endif// KYRNNESS_PLAYER_COMPONENT_HPP
