#pragma once
#ifndef KYRNESS_UISCALEBOX_HPP
#define KYRNESS_UISCALEBOX_HPP

#include "UI/Elements/UIElement.hpp"

class UUIScaleBox : public UUIElement
{
public:
    EScaleMode ScaleMode = EScaleMode::ScaleToFitBoth;
    glm::vec2 DesignResolution = glm::vec2(1280.0f, 720.0f);

    void UpdateLayout() override;

    glm::mat4 GetWorldModel() override;
    glm::vec2 GetCurrentScale() const { return CurrentScale; }

protected:
    glm::vec2 CurrentScale = glm::vec2(1.0f, 1.0f);

};

#endif// KYRNESS_UISCALEBOX_HPP