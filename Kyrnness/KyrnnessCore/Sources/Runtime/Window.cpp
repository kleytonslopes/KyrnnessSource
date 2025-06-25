#include "CorePCH.hpp"
#include "Runtime/Window.hpp"
#include "Runtime/Application.hpp"

UWindow::UWindow(UApplication* application) : m_Application(application)
{

}

UWindow::~UWindow()
{
}

uint32 UWindow::GetWidth() const
{
	return m_Application->GetWidth();
}

uint32 UWindow::GetHeight() const
{
	return m_Application->GetHeight();
}
