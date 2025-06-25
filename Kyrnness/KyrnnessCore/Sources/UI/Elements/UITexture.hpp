/*********************************************************************
 *   File: UITexture.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_UI_UITEXTURE_HPP
#define K_UI_UITEXTURE_HPP

#include "UI/Elements/UIElement.hpp"

class UUITexture : public UUIElement
{
public:
	void Draw() override;

	void HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp) override;
};

#endif// K_UI_UITEXTURE_HPP
