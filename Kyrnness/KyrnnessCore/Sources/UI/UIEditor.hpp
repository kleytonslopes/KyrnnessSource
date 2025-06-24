#pragma once
#ifndef KYRNNESS_UI_EDITOR_HPP
#define KYRNNESS_UI_EDITOR_HPP

#include "UI/HUD.hpp"
#include <memory>

class UApplication;

class UUIEditor : public UHUD
{
public:
	UUIEditor(UApplication* application);

	virtual void Initialize() override;

	virtual void Draw(float deltaTime) override;

private:
	
};

#endif // KYRNNESS_UI_EDITOR_HPP
