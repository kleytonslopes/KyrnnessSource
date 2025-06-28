/*********************************************************************
 *   File: GraphicsApi.cpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#include "CorePCH.hpp"
#include "Graphics/GraphicsApi.hpp"
#include "Runtime/Application.hpp"

UGraphicsApi::UGraphicsApi(UApplication* application)
	: m_Application (application)
{
	//m_Scene = m_Application->GetScene();
}

UGraphicsApi::~UGraphicsApi()
{

}
