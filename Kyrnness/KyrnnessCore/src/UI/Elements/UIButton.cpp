#include "pch.hpp"
#include "UI/Elements/UIButton.hpp"
#include "Runtime/Application.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Runtime/Application.hpp"

void UUIButton::Initialize()
{
	UUIElement::Initialize();

	SetTextureId(m_TextureIdle);
}

void UUIButton::HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp)
{
	int windowHeight = UApplication::Get().GetHeight();
	mouseY = windowHeight - mouseY;

	glm::vec2 totalScale = GetAccumulatedScale();

	float scaledMouseX = static_cast<float>(mouseX) / totalScale.x;
	float scaledMouseY = static_cast<float>(mouseY) / totalScale.y;

	bool insideX = scaledMouseX >= x && scaledMouseX <= (x + width);
	bool insideY = scaledMouseY >= y && scaledMouseY <= (y + height);

	if (insideX && insideY)
	{
		if (bHovered && m_MouseFocusState == EMouseFocusState::MFS_MouseEnter)
		{
			m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;
			
			if (OnClick && isMouseDown) OnClick();
		}
	}
}

void UUIButton::OnMouseEnter(double mouseX, double mouseY)
{
	UApplication::Get().GetSoundManager()->PlayUISound("button_hovered", 0.2f);
}

void UUIButton::OnMouseLeave(double mouseX, double mouseY)
{
	
}

void UUIButton::OnUpdateMouseFocus(double mouseX, double mouseY)
{
	Super::OnUpdateMouseFocus(mouseX, mouseY);

	if (bHovered)
		SetTextureId(m_TextureHovered);
	else
		SetTextureId(m_TextureIdle);
}

void UUIButton::SetFont(UUIFont* newFont)
{
	font = newFont;
}
