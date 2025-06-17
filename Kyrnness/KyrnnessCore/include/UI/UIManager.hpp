#pragma once
#ifndef KYRNESS_UI_UIMANAGER_HPP
#define KYRNESS_UI_UIMANAGER_HPP

#include <vector>
#include "UI/Elements/UIElement.hpp"


class UApplication;

class UUIManager
{
public:
	UUIManager(UApplication* application);
	~UUIManager();
	
	std::vector<UUIElement*> elements;

	void Initialize();

	void AddElement(UUIElement* e) { elements.push_back(e); }
	void RenderAll();
	void ProcessInput(float mx, float my, bool isMouseDown, bool isMouseUp);
	void OnMouseEnter(float mx, float my);
	void OnMouseLeave(float mx, float my);
	void OnUpdateMouseFocus(float mx, float my);
	


private:
	UApplication* m_Application = nullptr;
};

#endif// KYRNESS_UI_UIMANAGER_HPP
