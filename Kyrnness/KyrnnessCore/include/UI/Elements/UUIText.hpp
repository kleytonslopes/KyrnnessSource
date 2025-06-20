#pragma once
#ifndef KYRNESS_UUITEXT_HPP
#define KYRNESS_UUITEXT_HPP

#include "UI/Elements/UIElement.hpp"
#include "UI/UIFont.hpp"

enum class ETextHorizontalAlignment
{
    Left,
    Center,
    Right
};

enum class ETextVerticalAlignment
{
    Top,
    Center,
    Bottom
};

class UUIText : public UUIElement
{
public:
    UUIText();
    virtual ~UUIText();

    void Initialize();

    void SetText(const std::string& newText);
    void SetFont(UUIFont* newFont);
    void SetColor(const glm::vec4& color);
    void SetAutoFit(bool enabled);
    void SetWrapText(bool enabled);
    void SetHorizontalAlignment(ETextHorizontalAlignment align);
    void SetVerticalAlignment(ETextVerticalAlignment align);

    virtual void DrawSelf() override;
    virtual void UpdateLayout() override;

private:
    void GenerateMesh();

    std::string Text;
    UUIFont* Font = nullptr;
    glm::vec4 TextColor = glm::vec4(1.0f);

    bool bAutoFit = false;
    bool bWrapText = false;

    ETextHorizontalAlignment HorizontalAlignment = ETextHorizontalAlignment::Left;
    ETextVerticalAlignment VerticalAlignment = ETextVerticalAlignment::Top;

    GLuint m_VAO = 0;
    GLuint m_VBO = 0;

    std::vector<float> VertexBufferData;
    int VertexCount = 0;
};

#endif// KYRNESS_UUIBORDER_HPP
