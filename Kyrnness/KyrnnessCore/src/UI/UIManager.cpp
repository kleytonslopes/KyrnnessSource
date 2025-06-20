#include "pch.hpp"
#include "UI/UIManager.hpp"
#include "gl/GL.h"
#include "Runtime/Application.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include <entt/entt.hpp>
#include "Input/InputManager.hpp"
#include "Graphics/Shaders.hpp"
#include "UI/MinimalElements.hpp";

UUIManager::UUIManager(UApplication* application) : m_Application(application)
{
}

UUIManager::~UUIManager()
{
}

void UUIManager::Initialize()
{
	UApplication().Get().OnResolutionUpdatedEvent.AddListener(this, &UUIManager::OnResolutionUpdated);
	UInputManager::Get().OnMouseButtonEvent.AddListener(this, &UUIManager::ProcessInput);
	//UInputManager::Get().OnMouseMoveEvent.AddListener(this, &UUIManager::OnMouseEnter);
	UInputManager::Get().OnMouseMoveEvent.AddListener(this, &UUIManager::OnUpdateMouseFocus);
}

void UUIManager::UpdateElements()
{
	for (UUIElement* element : m_Elements)
	{
		element->UpdateLayout();
	}
}

void UUIManager::RenderAll()
{
	UpdateElements();

	for (auto& e : m_Elements)
		e->Draw();
}

void UUIManager::ProcessInput(float mx, float my, bool isMouseDown, bool isMouseUp)
{
	for (auto& e : m_Elements)
	{
		e->PropagateInput(mx, my, isMouseDown, isMouseUp);
	}
}

void UUIManager::OnMouseEnter(float mx, float my)
{
	for (auto& e : m_Elements)
	{
		e->PropagateMouseEnter(mx, my);
	}
}

void UUIManager::OnMouseLeave(float mx, float my)
{
	for (auto& e : m_Elements)
	{
		e->PropagateMouseLeave(mx, my);
	}
}

void UUIManager::OnUpdateMouseFocus(float mx, float my)
{
	// Garantindo que estamos usando o espa�o da janela
	int windowWidth = UApplication::Get().GetWidth();
	int windowHeight = UApplication::Get().GetHeight();

	// Opcional, s� se voc� quiser garantir clamping
	mx = glm::clamp(mx, 0.0f, (float)windowWidth);
	my = glm::clamp(my, 0.0f, (float)windowHeight);

	for (auto& e : m_Elements)
	{
		e->PropagateUpdateMouseFocus(mx, my);
	}
}

void UUIManager::OnResolutionUpdated(int width, int eight)
{
	UpdateLayoutAll();
}

void UUIManager::UpdateLayoutAll()
{
	for (auto& e : m_Elements)
	{
		e->UpdateLayout();
	}
}

UUIElement* UUIManager::CreateElementFromJson(const nlohmann::json& node)
{
	std::string type = node.value("Type", "");

	UUIElement* element = nullptr;

	if (type == "UUIBorder")
		element = FMemoryManager::Allocate<UUIBorder>();
	else if (type == "UUIButton")
		element = FMemoryManager::Allocate<UUIButton>();
	else if (type == "UUIText")
		element = FMemoryManager::Allocate<UUIText>();
	else if (type == "UIScaleBox")
		element = FMemoryManager::Allocate<UUIScaleBox>();
	// Adicione outros tipos conforme for criando

	if (element)
	{
		// Propriedades comuns
		element->Anchor = EAnchor::Center; //ParseAnchor(node.value("Anchor", "TopLeft"));
		element->width = node.value("Width", 100.0f);
		element->height = node.value("Height", 30.0f);
		element->OffsetX = node.value("OffsetX", 0.0f);
		element->OffsetY = node.value("OffsetY", 0.0f);
		element->LocalX = node.value("LocalX", 0.0f);
		element->LocalY = node.value("LocalY", 0.0f);

		// Textura (se existir)
		if (node.contains("Texture"))
		{
			GLuint tex = UAssetManager::LoadTextureOpenGL(node["Texture"].get<std::string>(), true);
			element->SetTextureId(tex);
			element->Initialize();
		}

		// Espec�fico para bot�o
		if (UUIButton* button = dynamic_cast<UUIButton*>(element))
		{
			//button->SetText(node.value("Text", ""));

			std::string onClickFunc = node.value("OnClick", "");
			if (!onClickFunc.empty())
			{
				//button->OnClickLuaFunction = onClickFunc;  // Voc� vai chamar via Lua mais tarde
			}
		}

		// Recurse em Children
		if (node.contains("Children"))
		{
			for (const auto& childNode : node["Children"])
			{
				if (UUIElement* child = CreateElementFromJson(childNode))
				{
					element->AddChild(child);
				}
			}
		}
	}


	return element;
}

// Texture loader
GLuint LoadTexture(const std::string& path);

GLuint quadVAO;
GLuint quadVBO;

void DrawQuad(float x, float y, float w, float h, GLuint texture, const glm::vec4 color)
{
	// 1. Inicializa VAO/VBO apenas uma vez
	if (quadVAO == 0)
	{
		float vertices[] = {
			// positions   // tex coords
			 0.0f, 0.0f,   0.0f, 0.0f,
			 1.0f, 0.0f,   1.0f, 0.0f,
			 1.0f, 1.0f,   1.0f, 1.0f,
			 0.0f, 1.0f,   0.0f, 1.0f
		};
		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		GLuint EBO;
		glGenBuffers(1, &EBO);

		glBindVertexArray(quadVAO);

		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// pos (vec2)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texcoord (vec2)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(w, h, 1.0f));

	glm::vec4 color2 = color;


	if (FShaderOpenGLComponent* shader = UShaders::GetShader(SHADER_UI))
	{
		shader->Bind();
		shader->SetMatrix4("projection", glm::ortho(0.0f, UApplication::Get().GetWidth<float>(), UApplication::Get().GetHeight<float>(), 0.0f));
		shader->SetMatrix4("model", model);
		shader->SetVector4("color", color);
		shader->SetInt("tex", 0); // texture unit 0

		GLint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		///std::cout << "Shader ativo ao desenhar texto: " << currentProgram << std::endl;

		// 4. Desenhar
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(quadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}