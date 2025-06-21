#include "pch.hpp"
#include "UI/Elements/UIButton.hpp"
#include "Runtime/Application.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Runtime/Application.hpp"

//extern void DrawQuad(float x, float y, float w, float h, GLuint tex, const glm::vec4 color);

//float vertices[] = {
//	// Posi��es (x, y)   // UVs (u, v)
// -0.5f, -0.5f,        0.0f, 0.0f,
//  0.5f, -0.5f,        1.0f, 0.0f,
// -0.5f,  0.5f,        0.0f, 1.0f,
//  0.5f,  0.5f,        1.0f, 1.0f
//};

void UUIButton::Initialize()
{
	UUIElement::Initialize();

	SetTextureId(m_TextureIdle);
}

void UUIButton::Draw()
{
	UUIElement::Draw();
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
	int windowHeight = UApplication::Get().GetHeight();

	bool insideX = mouseX >= x && mouseX <= (x + width);
	bool insideY = mouseY >= y && mouseY <= (y + height);

	if (insideX && insideY)
	{
		bHovered = true;

		if (bHovered && m_MouseFocusState != EMouseFocusState::MFS_MouseEnter)
		{
			//FLogger::Log("Mouse Entered UIButton: x= %f , y= %f", mouseX, mouseY);
			SetTextureId(m_TextureHovered);
			m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;
			if (OnHovered)
			{
				OnHovered(true);
			}
		}
	}
}

void UUIButton::OnMouseLeave(double mouseX, double mouseY)
{
	int windowHeight = UApplication::Get().GetHeight();

	bool insideX = mouseX >= x && mouseX <= (x + width);
	bool insideY = mouseY >= y && mouseY <= (y + height);

	if (!insideX || !insideY)
	{
		if (bHovered)
		{

			bHovered = false;
			SetTextureId(m_TextureIdle);
			if (!bHovered && m_MouseFocusState != EMouseFocusState::MFS_None)
			{
				//FLogger::Log("Mouse Leave UIButton: x= %f , y= %f", mouseX, mouseY);

				m_MouseFocusState = EMouseFocusState::MFS_None;
				if (OnHovered) OnHovered(false);
			}
		}
	}
}

void UUIButton::OnUpdateMouseFocus(double mouseX, double mouseY)
{
	Super::OnUpdateMouseFocus(mouseX, mouseY);

	if (bHovered)
		SetTextureId(m_TextureHovered);
	else
		SetTextureId(m_TextureIdle);

	//bool insideX = mouseX >= x && mouseX <= (x + width);
	//bool insideY = mouseY >= y && mouseY <= (y + height);
	//bool isInside = insideX && insideY;



	//if (isInside && !hovered)
	//{
	//	// Mouse acabou de entrar
	//	hovered = true;
	//	m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;
	//	FLogger::Log("Mouse Entered UIButton: x= %f , y= %f", mouseX, mouseY);

	//	if (OnHovered) OnHovered(true);
	//}
	//else if (!isInside && hovered)
	//{
	//	// Mouse acabou de sair
	//	hovered = false;
	//	m_MouseFocusState = EMouseFocusState::MFS_None;
	//	FLogger::Log("Mouse Leave UIButton: x= %f , y= %f", mouseX, mouseY);

	//	if (OnHovered) OnHovered(false);
	//}

}

void UUIButton::SetFont(UUIFont* newFont)
{
	font = newFont;
}
