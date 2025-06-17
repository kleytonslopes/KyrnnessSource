#pragma once
#ifndef KYRNESS_UI_UIBUTTON_HPP
#define KYRNESS_UI_UIBUTTON_HPP

#include "UI/Elements/UIElement.hpp"
#include "UI/UIFont.hpp"


class UUIButton : public UUIElement
{
public:
    std::string text;
    GLuint texIdle = 0, texHover = 0;
    bool hovered = false;
    bool pressed = false;

	FVector4 NormalColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

    std::function<void()> OnClick;
    std::function<void(bool)> OnHovered;

    void Initialize();

    void Draw() override;

    void HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp) override;
	void OnMouseEnter(double mouseX, double mouseY) override;
	void OnMouseLeave(double mouseX, double mouseY) override;
    void OnUpdateMouseFocus(double mouseX, double mouseY) override;

	void SetFont(UUIFont* newFont);
private:
	UUIFont* font = nullptr;
};

#endif// KYRNESS_UI_UIBUTTON_HPP
