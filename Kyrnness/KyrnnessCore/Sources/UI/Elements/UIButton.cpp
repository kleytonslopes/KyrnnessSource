#include "CorePCH.hpp"
#include "UI/Elements/UIButton.hpp"
#include "Runtime/Application.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Runtime/Application.hpp"

void UUIButton::Initialize()
{
	Super::Initialize();

	if(bEnabled)
		SetTextureId(m_TextureIdle);
	else
		SetTextureId(m_TextureDisabled);
}

void UUIButton::OnMouseEnter(double mouseX, double mouseY)
{
	Super::OnMouseEnter(mouseX, mouseY);

	UApplication::Get().GetSoundManager()->PlayUISound("button_hovered", 1.0f);
}

void UUIButton::OnUpdateMouseFocus(double mouseX, double mouseY)
{
	Super::OnUpdateMouseFocus(mouseX, mouseY);

	if (bHovered)
		SetTextureId(m_TextureHovered);
	else
	{
		if (bEnabled)
			SetTextureId(m_TextureIdle);
		else
			SetTextureId(m_TextureDisabled);
	}
}

void UUIButton::SetFont(UUIFont* newFont)
{
	font = newFont;
}
