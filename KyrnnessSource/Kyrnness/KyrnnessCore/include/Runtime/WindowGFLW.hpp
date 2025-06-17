#pragma once
#ifndef KYRNNES_CORE_RUNTIME_WINDOW_GLFW_HPP 
#define KYRNNES_CORE_RUNTIME_WINDOW_GLFW_HPP

#include "Runtime/Window.hpp"

class GLFWwindow;

class UWindowGLFW : public UWindow
{
public:
    UWindowGLFW(UApplication* application);

    UWindowGLFW(const UWindowGLFW&) = default;
    UWindowGLFW& operator=(const UWindowGLFW&) = default;
    UWindowGLFW(UWindowGLFW&&) = default;
    UWindowGLFW& operator=(UWindowGLFW&&) = default;

    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void PollEvents() override;
    virtual void WaitIdle() override;
    virtual bool ShouldClose() const override;
    virtual void SetTitle(const std::string& title) override;
    virtual void OnMouseMove(float x, float y) override;

	GLFWwindow* GetGLFWWindow() noexcept
	{
		return m_glfwWindow;
	}

private:
    GLFWwindow* m_glfwWindow = nullptr;

	float m_LastMouseX = 0.0f;
	float m_LastMouseY = 0.0f;

    bool bFirstMouse = true;

    void InitializeForOpenGL();
    void ProcessInput();
};

#endif // KYRNNES_CORE_RUNTIME_WINDOW_GLFW_HPP