#pragma once
#ifndef KYRNESS_UIUIBORDER_HPP
#define KYRNESS_UIUIBORDER_HPP

#include "UI/Elements/UIElement.hpp"

class UUIBorder : public UUIElement
{
public:
    GLuint texIdle = 0, texHover = 0;

    FVector4 NormalColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

    void Initialize();

    void Draw() override;


};

#endif// KYRNESS_UIUIBORDER_HPP
