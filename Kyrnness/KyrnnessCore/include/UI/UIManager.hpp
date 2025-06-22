#pragma once
#ifndef KYRNESS_UI_UIMANAGER_HPP
#define KYRNESS_UI_UIMANAGER_HPP

#include <vector>
#include "UI/Elements/UIElement.hpp"
#include "nlohmann/json.hpp"


class UApplication;

class UUIManager
{
public:
	UUIManager(UApplication* application);
	~UUIManager();
	


	void Initialize();

	void AddElement(UUIElement* element) { m_Elements.push_back(element); }
	void UpdateElements();
	void RenderAll();
	void ProcessInput(float mx, float my, bool isMouseDown, bool isMouseUp);
	void OnMouseEnter(float mx, float my);
	void OnMouseLeave(float mx, float my);
	void OnUpdateMouseFocus(float mx, float my);
	void OnResolutionUpdated(int width, int eight);
	void UpdateLayoutAll();
	void SetElementVisibility(const std::string& elementName, bool visible);
	UUIElement* CreateElementFromJson(const nlohmann::json& node);
	UUIElement* FindElementByName(const std::string& elementName);
	UUIElement* FindElementRecursive(UUIElement* element, const std::string& elementName);


private:
	UApplication* m_Application = nullptr;
	std::vector<UUIElement*> m_Elements;
	EAnchor ParseAnchor(const std::string& anchorStr);
};

#endif// KYRNESS_UI_UIMANAGER_HPP
