/*********************************************************************
 *   File: Controller.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef KYRNNES_CONTROLLER_HPP
#define	KYRNNES_CONTROLLER_HPP

#include "Core/Core.hpp"

class UApplication;

class UController
{
public:
	FFunctionEvent<int, int> OnKeyPressedEvent;
	FFunctionEvent<float, float> OnMouseMoveEvent;

	UController(UApplication* application);
	virtual ~UController() = default;

	// Disable copy and move semantics
	UController(const UController&) = delete;
	UController& operator=(const UController&) = delete;
	UController(UController&&) = delete;
	UController& operator=(UController&&) = delete;

	// Add any necessary methods or properties for the controller here
	virtual void Initialize();
	virtual void Update(float DeltaTime);
	virtual void Shutdown();

	void OnKeyPressed(int Key);
	void OnKeyReleased(int Key);

	void OnMouseMove(float x, float y);

private:
	UApplication* m_Application = nullptr;
	entt::entity m_Pawn = entt::null;
};

#endif // KYRNNES_CONTROLLER_HPP
