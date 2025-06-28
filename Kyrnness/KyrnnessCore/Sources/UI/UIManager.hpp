/*********************************************************************
 *   File: UIManager.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_UI_UIMANAGER_HPP
#define K_UI_UIMANAGER_HPP

#include <vector>
#include "UI/Elements/UIElement.hpp"
#include "Core/TypesDefinition.hpp"
#include "nlohmann/json.hpp"


class UApplication;

class UUIManager
{
public:
	UUIManager(UApplication* application);
	~UUIManager();



	void Initialize();

	void AddElement(UUIElement* element);
	void RemoveElement(UUIElement* element);
	void RemoveElementByName(const std::string& name);
	void UpdateElements();
	void RenderAll();
	void ClearAllElements();
	void ProcessInput(float mx, float my, bool isMouseDown, bool isMouseUp);
	void OnMouseEnter(float mx, float my);
	void OnMouseLeave(float mx, float my);
	void OnUpdateMouseFocus(float mx, float my);
	void OnResolutionUpdated(int width, int eight);
	void UpdateLayoutAll();
	void SetElementVisibility(const std::string& elementName, bool visible);
	void RegisterElementName(const std::string& name, UUIElement* element);
	void UnregisterElementName(const std::string& name);
	UUIElement* CreateElementFromJson(const nlohmann::json& node);
	UUIElement* FindElementByName(const std::string& elementName);
	UUIElement* FindElementRecursive(UUIElement* element, const std::string& elementName);

	template<typename T>
	void FindElementsOfType(std::vector<T*>& outElements);



private:
	UApplication* m_Application = nullptr;
	std::vector<UUIElement*> m_Elements;

	TMap<std::string, UUIElement*> m_ElementsCached;
	EAnchor ParseAnchor(const std::string& anchorStr);

	void BuildNameCacheRecursive(UUIElement* element);
	void ClearNameCache();
};

#endif// K_UI_UIMANAGER_HPP


