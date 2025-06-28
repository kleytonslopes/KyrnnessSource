/*********************************************************************
 *   File: UIButton.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_UI_UIBUTTON_HPP
#define K_UI_UIBUTTON_HPP

#include "UI/Elements/UIElement.hpp"
#include "UI/UIFont.hpp"


class UUIButton : public UUIElement
{
    using Super = UUIElement;
public:
    std::string text;
    //GLuint texIdle = 0, texHover = 0;


	FVector4 NormalColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

    void Initialize() override;

	void OnMouseEnter(double mouseX, double mouseY) override;
    void OnUpdateMouseFocus(double mouseX, double mouseY) override;

	void SetFont(UUIFont* newFont);

    void SetTextureIdle(unsigned int value) { m_TextureIdle = value; }
    void SetTextureHovered(unsigned int value) { m_TextureHovered = value; }
    void SetTextureDisabled(unsigned int value) { m_TextureDisabled = value; }
private:
	UUIFont* font = nullptr;

    unsigned int m_TextureIdle     = 0;
    unsigned int m_TextureHovered  = 0;
    unsigned int m_TextureDisabled = 0;
};

#endif// K_UI_UIBUTTON_HPP
