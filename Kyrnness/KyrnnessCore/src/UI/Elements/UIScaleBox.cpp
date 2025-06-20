#include "pch.hpp"
#include "UI/Elements/UIScaleBox.hpp"
#include "Runtime/Application.hpp"

void UUIScaleBox::UpdateLayout()
{
    UUIElement::UpdateLayout(); // Primeiro calcula posição própria

    float targetWidth = static_cast<float>(UApplication::Get().GetWidth());
    float targetHeight = static_cast<float>(UApplication::Get().GetHeight());

    float scaleX = targetWidth / DesignResolution.x;
    float scaleY = targetHeight / DesignResolution.y;

    switch (ScaleMode)
    {
    case EScaleMode::ScaleToFitX:
        CurrentScale = glm::vec2(scaleX, scaleX);
        break;

    case EScaleMode::ScaleToFitY:
        CurrentScale = glm::vec2(scaleY, scaleY);
        break;

    case EScaleMode::ScaleToFitBoth:
        CurrentScale = glm::vec2(glm::min(scaleX, scaleY), glm::min(scaleX, scaleY));
        break;

    case EScaleMode::None:
    default:
        CurrentScale = glm::vec2(1.0f, 1.0f);
        break;
    }

    for (UUIElement* child : Children)
    {
        if (child)
        {
            // Aqui é o segredo: Recalcula posição e tamanho a cada frame a partir do BaseX/BaseY/BaseWidth/BaseHeight
            child->x = child->BaseX * CurrentScale.x;
            child->y = child->BaseY * CurrentScale.y;
            child->width = child->BaseWidth * CurrentScale.x;
            child->height = child->BaseHeight * CurrentScale.y;

            child->UpdateLayout();
        }
    }
}

glm::mat4 UUIScaleBox::GetWorldModel()
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x + (width / 2), y + (height / 2), 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    if (Parent)
    {
        glm::mat4 parentModel = Parent->GetWorldModel();

        // Aplica a escala nos filhos, multiplicando no meio da hierarquia
        model = parentModel * model;
    }

    // Depois da transformação de posição/tamanho, aplicamos a escala
    model = glm::scale(model, glm::vec3(CurrentScale.x, CurrentScale.y, 1.0f));

    return model;
}
