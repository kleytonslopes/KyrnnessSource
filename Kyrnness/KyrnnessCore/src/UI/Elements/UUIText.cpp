#include "pch.hpp"
#include "UI/Elements/UUIText.hpp"
#include "Runtime/Application.hpp"
#include "Graphics/Shaders.hpp"
#include "Components/ShaderOpenGLComponent.hpp"

UUIText::UUIText()
{

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

UUIText::~UUIText()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void UUIText::Initialize()
{
	UUIElement::Initialize();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

void UUIText::SetText(const std::string& newText)
{
	Text = newText;
	GenerateMesh();
}

void UUIText::SetFont(UUIFont* newFont)
{
	Font = newFont;
	GenerateMesh();
}

void UUIText::SetColor(const glm::vec4& color)
{
	TextColor = color;
}

void UUIText::SetAutoFit(bool enabled)
{
	bAutoFit = enabled;
	GenerateMesh();
}

void UUIText::SetWrapText(bool enabled)
{
	bWrapText = enabled;
	GenerateMesh();
}

void UUIText::SetHorizontalAlignment(ETextHorizontalAlignment align)
{
	HorizontalAlignment = align;
	GenerateMesh();
}

void UUIText::SetVerticalAlignment(ETextVerticalAlignment align)
{
	VerticalAlignment = align;
	GenerateMesh();
}

void UUIText::UpdateLayout()
{
	UUIElement::UpdateLayout();
	GenerateMesh();
}

void UUIText::GenerateMesh()
{
	VertexBufferData.clear();

	if (!Font || Text.empty())
		return;

	float cursorX = 0.0f;
	float cursorY = 0.0f;

	// Placeholder: Aqui futuramente você vai consultar o Font->GetGlyph(), Font->GetKerning(), etc.
	for (char c : Text)
	{
		// Exemplo fictício:
		float glyphWidth = 10.0f;
		float glyphHeight = 20.0f;
		float u0 = 0.0f;
		float v0 = 0.0f;
		float u1 = 1.0f;
		float v1 = 1.0f;

		// Calcula quad do caractere
		float x0 = cursorX;
		float y0 = cursorY;
		float x1 = cursorX + glyphWidth;
		float y1 = cursorY + glyphHeight;

		// Posição + UVs (triângulo 1)
		VertexBufferData.insert(VertexBufferData.end(), {
			x0, y0, u0, v0,
			x1, y0, u1, v0,
			x0, y1, u0, v1,

			// Triângulo 2
			x1, y0, u1, v0,
			x1, y1, u1, v1,
			x0, y1, u0, v1
			});

		cursorX += glyphWidth; // Simulação do avanço
	}

	VertexCount = static_cast<int>(VertexBufferData.size() / 4);

	// Upload para o VBO
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferData.size() * sizeof(float), VertexBufferData.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UUIText::DrawSelf()
{
	if (!Font || VertexCount == 0)
		return;

	if (FShaderOpenGLComponent* shader = UShaders::GetShader(SHADER_UI_TEXT))
	{
		shader->Bind();
		shader->SetMatrix4("uProjection", GetProjetion());
		shader->SetMatrix4("uModel", GetWorldModel());
		shader->SetVector4("uColor", TextColor);
		shader->SetInt("uTexture", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Font->GetTextureId());

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, VertexCount);
		glBindVertexArray(0);
	}
}
