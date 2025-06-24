#pragma once
#ifndef KYRNESS_UI_UITEXTURE_HPP
#define KYRNESS_UI_UITEXTURE_HPP

#include "UI/Elements/UIElement.hpp"

class UUITexture : public UUIElement
{
public:
	void Draw() override;

	void HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp) override;
};

#endif// KYRNESS_UI_UITEXTURE_HPP
