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

void UUIElement::AddChild(UUIElement* child)
{
    if (child)
    {
        child->Parent = this;
        Children.push_back(child);
    }
}

void UUIElement::Initialize()
{
    BaseX = x;
    BaseY = y;
    BaseWidth = width;
    BaseHeight = height;
}

void UUIElement::Draw()
{
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

    DrawSelf(); // Cada tipo de elemento (ex: Border, Button) faz seu desenho aqui

    for (UUIElement* child : Children)
    {
        if (child)
        {
            child->Draw();
        }
    }

}

void UUIElement::UpdateLayout()
{
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

    // Aplica margem para os outros Anchors (exceto Stretch que já aplicou antes)
    if (Anchor != EAnchor::Stretch)
    {
        baseX += Margin.Left - Margin.Right;
        baseY += Margin.Top - Margin.Bottom;
    }

    // A posição final (absoluta) é: Anchor + Offset + LocalX/LocalY
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
        if (child)
        {
            child->UpdateLayout();
        }
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

void UUIElement::DrawSelf()
{
    if (m_TextureID != 0)
    {
        glm::mat4 model = GetWorldModel();
        glm::mat4 projection = GetProjetion();

        // Aqui você seta os uniforms no shader
        auto shaderEntities = UApplication::Get().GetEnttRegistry().view<FShaderOpenGLComponent>();
        shaderEntities.each([&](const auto entity, auto& shader)
            {
                if (shader.GetShaderName() == "uiShader")
                {
                    shader.Bind();
                    shader.SetMatrix4("projection", projection);
                    shader.SetMatrix4("model", model);
                    shader.SetVector4("color", glm::vec4(1, 1, 1, 1));
                    shader.SetInt("tex", 0);
                }
            });

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glBindVertexArray(m_VAO);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
