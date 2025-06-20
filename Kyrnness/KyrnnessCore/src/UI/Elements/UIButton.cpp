#include "pch.hpp"
#include "UI/Elements/UIButton.hpp"
#include "Runtime/Application.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Runtime/Application.hpp"

extern void DrawQuad(float x, float y, float w, float h, GLuint tex, const glm::vec4 color);

float vertices[] = {
	// Posições (x, y)   // UVs (u, v)
 -0.5f, -0.5f,        0.0f, 0.0f,
  0.5f, -0.5f,        1.0f, 0.0f,
 -0.5f,  0.5f,        0.0f, 1.0f,
  0.5f,  0.5f,        1.0f, 1.0f
};

void UUIButton::Initialize()
{
	UUIElement::Initialize();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

void UUIButton::Draw()
{
	UUIElement::Draw();
	if (!m_Shader)
		return;

	m_Shader->Bind();
	m_Shader->SetMatrix4("uProjection", GetProjetion());
	m_Shader->SetMatrix4("uModel", GetModel());
	m_Shader->SetInt("uTexture", 0);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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
		if (hovered && m_MouseFocusState == EMouseFocusState::MFS_MouseEnter)
		{
			m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;
			if (OnClick && isMouseDown) OnClick();
		}
	}

	//int windowHeight = UApplication::Get().GetHeight();
	//mouseY = windowHeight - mouseY;

	////LOG(Log, TEXT("Mouse Want Click UIButton: x= %f , y= %f", mouseX, mouseY));


	//bool insideX = mouseX >= x && mouseX <= (x + width);
	//bool insideY = mouseY >= y && mouseY <= (y + height);

	//if (insideX && insideY)
	//{
	//	if (hovered && m_MouseFocusState == EMouseFocusState::MFS_MouseEnter)
	//	{
	//		m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;
	//		if (OnClick && isMouseDown) OnClick();
	//	}
	//}
}

void UUIButton::OnMouseEnter(double mouseX, double mouseY)
{
	int windowHeight = UApplication::Get().GetHeight();

	bool insideX = mouseX >= x && mouseX <= (x + width);
	bool insideY = mouseY >= y && mouseY <= (y + height);

	if (insideX && insideY)
	{
		hovered = true;

		if (hovered && m_MouseFocusState != EMouseFocusState::MFS_MouseEnter)
		{
			//LOG(Log, TEXT("Mouse Entered UIButton: x= %f , y= %f", mouseX, mouseY));

			m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;
			if (OnHovered) OnHovered(true);
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
		if (hovered)
		{

			hovered = false;

			if (!hovered && m_MouseFocusState != EMouseFocusState::MFS_None)
			{
				//LOG(Log, TEXT("Mouse Leave UIButton: x= %f , y= %f", mouseX, mouseY));

				m_MouseFocusState = EMouseFocusState::MFS_None;
				if (OnHovered) OnHovered(false);
			}
		}
	}
}

void UUIButton::OnUpdateMouseFocus(double mouseX, double mouseY)
{
	bool insideX = mouseX >= x && mouseX <= (x + width);
	bool insideY = mouseY >= y && mouseY <= (y + height);
	bool isInside = insideX && insideY;



	if (isInside && !hovered)
	{
		// Mouse acabou de entrar
		hovered = true;
		m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;
		LOG(Log, TEXT("Mouse Entered UIButton: x= %f , y= %f", mouseX, mouseY));

		if (OnHovered) OnHovered(true);
	}
	else if (!isInside && hovered)
	{
		// Mouse acabou de sair
		hovered = false;
		m_MouseFocusState = EMouseFocusState::MFS_None;
		LOG(Log, TEXT("Mouse Leave UIButton: x= %f , y= %f", mouseX, mouseY));

		if (OnHovered) OnHovered(false);
	}

}

void UUIButton::SetFont(UUIFont* newFont)
{
	font = newFont;
}
