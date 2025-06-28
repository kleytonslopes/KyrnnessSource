/*********************************************************************
 *   File: UIEditor.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_UI_EDITOR_HPP
#define K_UI_EDITOR_HPP

#include "UI/HUD.hpp"
#include <memory>

class UApplication;

class UUIEditor : public UHUD
{
public:
	UUIEditor(UApplication* application);

	virtual void Initialize() override;

	virtual void Draw(float deltaTime) override;

private:
	
};

#endif // K_UI_EDITOR_HPP
