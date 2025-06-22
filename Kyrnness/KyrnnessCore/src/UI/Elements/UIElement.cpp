#include "pch.hpp"
#include "UI/Elements/UIElement.hpp"
#include "Components/ShaderOpenGLComponent.hpp"
#include "Graphics/Shaders.hpp"
#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"
#include "Runtime/Application.hpp"
#include "UI/Elements/UIScaleBox.hpp"



UUIElement::UUIElement()
{
	m_Shader = UShaders::GetShader(SHADER_UI);

#if(DEBUG)
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
#endif
}

void UUIElement::SetName(const std::string& newName)
{
	if (Parent)
	{
		if (UUIManager* uiManager = UApplication::Get().GetUIManager())
		{
			// Remove o nome antigo
			if (!m_Name.empty())
				uiManager->UnregisterElementName(m_Name);

			m_Name = newName;

			// Reindexa o novo nome
			if (!m_Name.empty())
				uiManager->RegisterElementName(m_Name, this);
		}
	}
	else
	{
		// Se for root, mesma lógica
		if (UUIManager* uiManager = UApplication::Get().GetUIManager())
		{
			if (!m_Name.empty())
				uiManager->UnregisterElementName(m_Name);

			m_Name = newName;

			if (!m_Name.empty())
				uiManager->RegisterElementName(m_Name, this);
		}
	}
}

void UUIElement::AddChild(UUIElement* child)
{
	if (child)
	{
		child->Parent = this;
		Children.push_back(child);
	}
}

void UUIElement::RemoveChild(UUIElement* child)
{
	auto it = std::find(Children.begin(), Children.end(), child);
	if (it != Children.end())
	{
		Children.erase(it);
	}
}

void UUIElement::Initialize()
{
	Super::Initialize();

	BaseX = x;
	BaseY = y;
	BaseWidth = width;
	BaseHeight = height;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

void UUIElement::OnDestroy()
{
	Super::OnDestroy();

	for (UUIElement* child : Children)
	{
		if (child)
			child->Destroy();
	}

	Children.clear();

	// Remover do cache
	if (!m_Name.empty())
	{
		UUIManager* uiManager = UApplication::Get().GetUIManager();
		if (uiManager)
			uiManager->UnregisterElementName(m_Name);
	}
}

void UUIElement::Draw()
{
	if (!IsValid())
		return;

	if (!bVisible)
		return;

#if(DEBUG)
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
#endif

	if (!m_Shader)
		return;

	if (Children.size() > 0)
	{
		for (auto& child : Children)
		{
			child->Draw();
		}
	}

	m_Shader->Bind();
	m_Shader->SetMatrix4("uProjection", GetProjetion());
	m_Shader->SetMatrix4("uModel", GetModel());
	m_Shader->SetInt("uTexture", 0);
	m_Shader->SetVector4("uColor", glm::vec4{ 1.f,1.f, 1.f, 1.f });



	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glBindVertexArray(m_VAO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	m_Shader->Unbind();

}

void UUIElement::UpdateLayout()
{
	if (!IsValid())
		return;

	bool isRoot = (Parent == nullptr);

	float parentX = 0.0f;
	float parentY = 0.0f;
	float parentW = static_cast<float>(UApplication::Get().GetWidth());
	float parentH = static_cast<float>(UApplication::Get().GetHeight());

	if (!isRoot)
	{
		parentX = Parent->x;
		parentY = Parent->y;
		parentW = Parent->width;
		parentH = Parent->height;
	}

	float baseX = parentX;
	float baseY = parentY;
	float finalWidth = width;
	float finalHeight = height;

	switch (Anchor)
	{
	case EAnchor::TopLeft:
		baseX = parentX;
		baseY = parentY;
		break;

	case EAnchor::TopCenter:
		baseX = parentX + (parentW - width) * 0.5f;
		baseY = parentY;
		break;

	case EAnchor::TopRight:
		baseX = parentX + (parentW - width);
		baseY = parentY;
		break;

	case EAnchor::CenterLeft:
		baseX = parentX;
		baseY = parentY + (parentH - height) * 0.5f;
		break;

	case EAnchor::Center:
		baseX = parentX + (parentW - width) * 0.5f;
		baseY = parentY + (parentH - height) * 0.5f;
		break;

	case EAnchor::CenterRight:
		baseX = parentX + (parentW - width);
		baseY = parentY + (parentH - height) * 0.5f;
		break;

	case EAnchor::BottomLeft:
		baseX = parentX;
		baseY = parentY + (parentH - height);
		break;

	case EAnchor::BottomCenter:
		baseX = parentX + (parentW - width) * 0.5f;
		baseY = parentY + (parentH - height);
		break;

	case EAnchor::BottomRight:
		baseX = parentX + (parentW - width);
		baseY = parentY + (parentH - height);
		break;

	case EAnchor::Stretch:
		baseX = parentX + Margin.Left;
		baseY = parentY + Margin.Top;
		finalWidth = parentW - (Margin.Left + Margin.Right);
		finalHeight = parentH - (Margin.Top + Margin.Bottom);
		break;
	}

	// Aplica margem para os outros Anchors (exceto Stretch que j� aplicou antes)
	if (Anchor != EAnchor::Stretch)
	{
		baseX += Margin.Left - Margin.Right;
		baseY += Margin.Top - Margin.Bottom;
	}

	// A posi��o final (absoluta) �: Anchor + Offset + LocalX/LocalY
	x = baseX + OffsetX + LocalX;
	y = baseY + OffsetY + LocalY;

	// Ajusta o tamanho final
	if (Anchor == EAnchor::Stretch)
	{
		width = finalWidth;
		height = finalHeight;
	}

	// Recursivo pros filhos
	for (UUIElement* child : Children)
	{
		try
		{
			if (child)
			{
				child->UpdateLayout();
			}

		}
		catch (const std::exception&)
		{
			FLogger::Fatal("Failed to UpdateLayout: %s", m_Name.c_str());
		}
	}
}

void UUIElement::HandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp)
{
	if (!bVisible || !bEnabled)
		return;

	OnHandleInput(mouseX, mouseY, isMouseDown, isMouseUp);
}

void UUIElement::MouseEnter(double mouseX, double mouseY)
{
	if (!bVisible || !bEnabled)
		return;

	OnMouseEnter(mouseX, mouseY);
}

void UUIElement::MouseLeave(double mouseX, double mouseY)
{
	if (!bVisible || !bEnabled)
		return;

	OnMouseLeave(mouseX, mouseY);
}

void UUIElement::UpdateMouseFocus(double mouseX, double mouseY)
{
	if (!bVisible || !bEnabled)
		return;

	OnUpdateMouseFocus(mouseX, mouseY);
}

void UUIElement::OnMouseLeave(double mouseX, double mouseY)
{
	if (!bVisible || !bEnabled)
		return;
}

void UUIElement::OnUpdateMouseFocus(double mouseX, double mouseY)
{
	if (!bVisible || !bEnabled)
		return;

	bool insideX = mouseX >= x && mouseX <= (x + width);
	bool insideY = mouseY >= y && mouseY <= (y + height);
	bool isInside = insideX && insideY;

	if (isInside && !bHovered)
	{
		MouseEnter(mouseX, mouseY);
		bHovered = true;
		m_MouseFocusState = EMouseFocusState::MFS_MouseEnter;

		if (OnHovered) OnHovered(true);
	}
	else if (!isInside && bHovered)
	{
		MouseLeave(mouseX, mouseY);
		bHovered = false;
		m_MouseFocusState = EMouseFocusState::MFS_None;

		if (OnHovered) OnHovered(false);
	}
}

void UUIElement::PropagateInput(float mx, float my, bool isMouseDown, bool isMouseUp)
{
	HandleInput(mx, my, isMouseDown, isMouseUp);

	for (UUIElement* child : Children)
	{
		if (child)
		{
			child->PropagateInput(mx, my, isMouseDown, isMouseUp);
		}
	}
}

void UUIElement::PropagateMouseEnter(double mouseX, double mouseY)
{
	OnMouseEnter(mouseX, mouseY);

	for (UUIElement* child : Children)
	{
		if (child)
		{
			child->PropagateMouseEnter(mouseX, mouseY);
		}
	}
}

void UUIElement::PropagateMouseLeave(double mouseX, double mouseY)
{
	OnMouseLeave(mouseX, mouseY);

	for (UUIElement* child : Children)
	{
		if (child)
		{
			child->PropagateMouseLeave(mouseX, mouseY);
		}
	}
}

void UUIElement::PropagateUpdateMouseFocus(double mouseX, double mouseY)
{
	OnUpdateMouseFocus(mouseX, mouseY);

	for (UUIElement* child : Children)
	{
		if (child)
		{
			child->PropagateUpdateMouseFocus(mouseX, mouseY);
		}
	}
}

glm::mat4 UUIElement::GetWorldModel()
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x + (width / 2), y + (height / 2), 0.0f));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	if (Parent)
	{
		return Parent->GetWorldModel() * model;
	}
	else
	{
		return model;
	}
}

glm::vec2 UUIElement::GetAccumulatedScale()
{
	glm::vec2 totalScale(1.0f, 1.0f);

	UUIElement* current = this;
	while (current != nullptr)
	{
		if (UUIScaleBox* scaleBox = dynamic_cast<UUIScaleBox*>(current))
		{
			totalScale.x *= scaleBox->GetCurrentScale().x;
			totalScale.y *= scaleBox->GetCurrentScale().y;
		}
		current = current->Parent;
	}
	return totalScale;
}

void UUIElement::OnHandleInput(double mouseX, double mouseY, bool isMouseDown, bool isMouseUp)
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

void UUIElement::OnMouseEnter(double mouseX, double mouseY)
{
	if (!bVisible || !bEnabled)
		return;
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
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x + (width / 2), y + (height / 2), 0.0f));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	return model;
}

void UUIElement::DrawSelf()
{
	if (!bVisible)
		return;

	if (m_TextureID != 0 && m_VAO != 0)
	{
		glm::mat4 model = GetWorldModel();
		glm::mat4 projection = GetProjetion();

		if (UShaderOpenGLComponent* shader = UShaders::GetShader(SHADER_UI))
		{
			shader->Bind();
			shader->SetMatrix4("uProjection", projection);
			shader->SetMatrix4("uModel", model);
			shader->SetVector4("uColor", glm::vec4(1, 1, 1, 1));
			shader->SetInt("tex", 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
			glBindVertexArray(m_VAO);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}

