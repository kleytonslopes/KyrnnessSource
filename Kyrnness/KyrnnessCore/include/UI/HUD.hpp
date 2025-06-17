#pragma once
#ifndef KYRNNESS_HUD_HPP
#define KYRNNESS_HUD_HPP

#include "Core/Core.hpp" 
#include "UI/UIManager.hpp"

class UApplication;

class UHUD
{
public:
	UHUD(UApplication* application);
	~UHUD() {};

	virtual void Initialize();
	virtual void PostInitialize();

	virtual void Draw(float deltaTime);

	UUIManager* GetUIManager() const { return m_UIManager.get(); }

protected:
	UApplication* m_Application;
	std::unique_ptr<UUIManager> m_UIManager;
};

#endif // KYRNNESS_HUD_HPP
