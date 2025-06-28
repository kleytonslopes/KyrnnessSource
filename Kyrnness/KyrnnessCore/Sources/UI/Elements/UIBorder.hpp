/*********************************************************************
 *   File: UIBorder.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_UUIBORDER_HPP
#define K_UUIBORDER_HPP

#include "UI/Elements/UIElement.hpp"

class UUIBorder : public UUIElement
{
public:
    GLuint texIdle = 0, texHover = 0;

    FVector4 NormalColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

    void Initialize();

    void Draw() override;


};

#endif// K_UUIBORDER_HPP
