#include "pch.hpp"
#include "UI/Elements/UIBorder.hpp"
#include "Components/ShaderOpenGLComponent.hpp"

extern void DrawQuad(float x, float y, float w, float h, GLuint tex, const glm::vec4 color);

void UUIBorder::Initialize()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

void UUIBorder::Draw()
{
	UUIElement::Draw();
	if (!m_Shader)
		return;

	m_Shader->Bind();
	m_Shader->SetMatrix4("uProjection", GetProjetion());
	m_Shader->SetMatrix4("uModel", GetModel());
	m_Shader->SetInt("uTexture", 0);

	float vertices[] = {
		// Posições (x, y)   // UVs (u, v)
	 -0.5f, -0.5f,        0.0f, 0.0f,
	  0.5f, -0.5f,        1.0f, 0.0f,
	 -0.5f,  0.5f,        0.0f, 1.0f,
	  0.5f,  0.5f,        1.0f, 1.0f
	};

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

void UUIBorder::HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp)
{
}

void UUIBorder::OnMouseEnter(double mouseX, double mouseY)
{
}

void UUIBorder::OnMouseLeave(double mouseX, double mouseY)
{
}

void UUIBorder::OnUpdateMouseFocus(double mouseX, double mouseY)
{
}
