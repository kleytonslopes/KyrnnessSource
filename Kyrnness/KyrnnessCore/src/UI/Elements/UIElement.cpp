#include "pch.hpp"
#include "UI/Elements/UIElement.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Graphics/Shaders.hpp"
#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"
#include "Runtime/Application.hpp"

UUIElement::UUIElement()
{
	m_Shader = UShaders::GetShader(SHADER_UI);
	m_ShaderDebug = UShaders::GetShader(SHADER_UI_DEBUG);

	glGenVertexArrays(1, &m_VAO_Debug);
	glGenBuffers(1, &m_VBO_Debug);

	glGenVertexArrays(1, &m_VAO_Debug);
	glGenBuffers(1, &m_VBO_Debug);

	glBindVertexArray(m_VAO_Debug);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Debug);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UUIElement::Draw()
{
	if (UGraphicsApi_OpenGL* api = UApplication::Get().GetGraphicsApi<UGraphicsApi_OpenGL>())
	{
		glm::vec4 btnColor = { 1,1,1,1 };
		glm::vec3 btn_Start{ x, y, 0 };
		glm::vec3 btn_End{ x + width, y + height, 0 };

		glm::vec3 btn_Start2{ x, y + height, 0 };
		glm::vec3 btn_End2{ x + width, y, 0 };

		api->DebugDrawLine2D(btn_Start, btn_End, btnColor);
		api->DebugDrawLine2D(btn_Start2, btn_End2, btnColor);
	}

}

glm::mat4 UUIElement::GetProjetion()
{
	return glm::ortho(0.0f
		, static_cast<float>(UApplication::Get().GetWidth())
		, static_cast<float>(UApplication::Get().GetHeight())
		, 0.0f
		, -1.0f
		, 1.0f);
}

glm::mat4 UUIElement::GetModel()
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x + (width/ 2), y + (height / 2), 0.0f));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	return model;
}