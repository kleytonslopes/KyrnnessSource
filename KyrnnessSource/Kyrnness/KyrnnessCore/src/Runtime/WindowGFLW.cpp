#include "pch.hpp"
#include "Runtime/WindowGFLW.hpp"
#include "Runtime/Application.hpp"
#include "Input/InputManager.hpp"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);

}

UWindowGLFW::UWindowGLFW(UApplication* application) : UWindow(application)
{

}

void UWindowGLFW::Initialize()
{
	if (!glfwInit())
	{
		ThrowRuntimeError("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_glfwWindow = glfwCreateWindow(GetWidth(), GetHeight(), "Kyrnness OpenGL", nullptr, nullptr);
	if (!m_glfwWindow)
	{
		glfwTerminate();
		ThrowRuntimeError("Failed to create window");
	}

	glfwMakeContextCurrent(m_glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ThrowRuntimeError("Failed to initialize GLAD");
	}

	//glfwSetInputMode(m_glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glfwSetWindowUserPointer(m_glfwWindow, this);
	glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
		{
			auto* self = static_cast<UWindowGLFW*>(glfwGetWindowUserPointer(window));
			if (self)
				self->OnMouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
		});
}

void UWindowGLFW::Destroy()
{
	glfwTerminate();

	LOG(Warning, "UWindowGLFW::Destroy");
}

void UWindowGLFW::PollEvents()
{
	ProcessInput();

	glfwSwapBuffers(m_glfwWindow);
	glfwPollEvents();
}

void UWindowGLFW::WaitIdle()
{

}

bool UWindowGLFW::ShouldClose() const
{
	return glfwWindowShouldClose(m_glfwWindow);
}

void UWindowGLFW::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(m_glfwWindow, title.c_str());
}

void UWindowGLFW::OnMouseMove(float x, float y)
{
	if (bFirstMouse)
	{
		m_LastMouseX = x;
		m_LastMouseY = y;
		bFirstMouse = false;
	}

	float deltaX = x - m_LastMouseX;
	float deltaY = m_LastMouseY - y;

	m_LastMouseX = x;
	m_LastMouseY = y;

	deltaX *= 0.01f; // Adjust sensitivity as needed
	deltaY *= 0.01f; // Adjust sensitivity as needed

	UInputManager::GetInstance().OnMouseMove(deltaX, deltaY);
}

void UWindowGLFW::InitializeForOpenGL()
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_glfwWindow = glfwCreateWindow(GetWidth(), GetHeight(), "Kyrnness OpenGL", nullptr, nullptr);

	glfwMakeContextCurrent(m_glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ThrowRuntimeError("Failed to initialize GLAD with GLFW");
	}
}

void UWindowGLFW::ProcessInput()
{
	if (glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_glfwWindow, true);

	for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
	{
		int state = glfwGetKey(m_glfwWindow, key);

		if (state == GLFW_PRESS)	    UInputManager::GetInstance().SetKeyPressed(key, EInputKeyStatus::IKS_Pressed);
		else if (state == GLFW_RELEASE) UInputManager::GetInstance().SetKeyPressed(key, EInputKeyStatus::IKS_Released);

		//OnKeyPressedEvent.Broadcast(key, state);
	}
}
