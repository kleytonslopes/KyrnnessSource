#include "pch.hpp"
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
