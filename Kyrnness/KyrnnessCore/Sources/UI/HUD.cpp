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


UHUD::UHUD(UApplication* application) : m_Application(application)
{

}

void UHUD::Initialize()
{
	//m_UIManager = std::make_unique<UUIManager>(m_Application);
	//m_UIManager->Initialize();
}

void UHUD::PostInitialize()
{
	//if (!m_UIManager)
	//{
	//	ThrowRuntimeError("UI Manager not Initialized");
	//}
}

void UHUD::Draw(float deltaTime)
{
	//if (m_UIManager)
	//{
	//	m_UIManager->RenderAll();
	//}
}