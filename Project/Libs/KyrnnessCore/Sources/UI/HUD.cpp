/*********************************************************************
 *   File: HUD.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "UI/HUD.hpp"
#include "Runtime/Application.hpp"
#include "Graphics/OpenGL/GraphicsApi_OpenGL.hpp"

//
//UHUD::UHUD(UApplication* application) : m_Application(application)
//{
//
//}

void UHUD::SetHUDPath(const std::string& hudFilePath)
{
	m_HudFilePath = hudFilePath;
}

void UHUD::PreInitialize()
{
	Super::PreInitialize();

	if (m_HudFilePath == "")
		FException::InvalidArgument("Hud file Path not set!");

	nlohmann::json uiJson = UAssetManager::LoadJson(m_HudFilePath);

	UUIElement* rootElement = UApplication::Get().GetUIManager()->CreateElementFromJson(uiJson);
	UApplication::Get().GetUIManager()->AddElement(rootElement);
}

void UHUD::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	Draw(DeltaTime);
}

void UHUD::Draw(float deltaTime)
{
	UGraphicsApi_OpenGL* api = UApplication::Get().GetGraphicsApi<UGraphicsApi_OpenGL>();

	//UHUD::Draw(deltaTime);


	glm::vec4 colorX = { 1,0,0,1 };
	glm::vec4 colorY = { 0,1,0,1 };

	glm::vec3 fromX{ 1, 1.f, 0 };
	glm::vec3 fromX2{ 100, 1.f, 0 };

	glm::vec3 fromY{ 1, 1, 0 };
	glm::vec3 fromY2{ 1, 100.f, 0 };

#ifdef DEBUG
	api->DebugDrawLine2D(fromX, fromX2, colorX);
	api->DebugDrawLine2D(fromY, fromY2, colorY);
#endif // DEBUG
}
