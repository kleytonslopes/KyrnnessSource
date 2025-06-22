#pragma once
#ifndef GAME_HUD_HPP
#define GAME_HUD_HPP

#include "UI/HUD.hpp"

class UApplication;
class UShaderOpenGLComponent;

class UGameHUD : public UHUD
{

public:
	UGameHUD(UApplication* application);

	void Initialize() override;

	void Draw(float deltaTime) override;

};
#endif // !GAME_HUD_HPP

